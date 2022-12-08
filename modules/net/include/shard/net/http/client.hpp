// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_HTTP_CLIENT_HPP
#define SHARD_HTTP_CLIENT_HPP

#include "shard/net/curl/handle.hpp"
#include "shard/net/http/future.hpp"
#include "shard/net/http/request.hpp"
#include "shard/net/http/response.hpp"
#include "shard/net/http/version.hpp"

#include <shard/concurrency/channel.hpp>

#include <cstddef>
#include <mutex>
#include <thread>

namespace shard::net::http {

class client {
public:
    /// Default constructor that starts the worker thread
    explicit client(version_t version = version_t::http_2) : m_thread(&client::worker_thread, this) {
        setup_curl_handle(version);
    };

    /// Destructor that joins the worker thread
    ~client() {
        m_requests.close();
        m_thread.join();
    }

    /// Get the request timeout
    std::size_t timeout() const { return m_timeout; }

    /// Set the request timeout
    void set_timeout(std::size_t value) { m_timeout = value; }

    /// Get the connection timeout
    std::size_t connection_timeout() const { return m_connection_timeout; }

    /// Set the connection timeout
    void set_connection_timeout(std::size_t value) { m_connection_timeout = value; }

    /// Send the HTTP request
    ///
    /// \note This does not guarantee that the request is sent immediately.
    ///       The request is placed in a queue that is processed by the
    ///       background thread(s).
    ///
    /// \return A future object to add a callback to the request or block until
    ///         the response is available or the request times out.
    future send_request(request::ptr request) {
        auto state = std::make_shared<detail::shared_state>();
        request->m_state = state;
        m_requests.push(std::move(request));
        return future {std::move(state)};
    }

private:
    bool setup_curl_handle(version_t version) {
        std::lock_guard<std::mutex> curl_lock(m_mutex);

        bool result = m_curl.set_option<CURLOPT_HTTP_VERSION>(version);
        result = result && m_curl.set_option<CURLOPT_WRITEFUNCTION>(&client::write_callback);
        result = result && m_curl.set_option<CURLOPT_HEADERFUNCTION>(&client::write_callback);
        result = result && m_curl.set_option<CURLOPT_TIMEOUT>(m_timeout);
        result = result && m_curl.set_option<CURLOPT_CONNECTTIMEOUT>(m_connection_timeout);

        return result;
    }

    void reset_curl_handle() {
        m_curl.set_option<CURLOPT_HTTPGET>(false);
        m_curl.set_option<CURLOPT_FOLLOWLOCATION>(true);
        m_curl.set_option<CURLOPT_POST>(false);
        m_curl.set_option<CURLOPT_PUT>(false);
        m_curl.set_option<CURLOPT_NOBODY>(false);
        m_curl.set_option<CURLOPT_POSTFIELDSIZE>(0);
        m_curl.set_option<CURLOPT_POSTFIELDS>(std::nullopt);
        m_curl.set_option<CURLOPT_COPYPOSTFIELDS>(std::nullopt);
        m_curl.set_option<CURLOPT_CUSTOMREQUEST>(std::nullopt);
        m_curl.set_option<CURLOPT_HTTPHEADER>(curl::slist {});
    }

    void worker_thread() {
        while (auto request = m_requests.pop()) {
            perform_request(std::move(*request));
        }
    }

    void perform_request(request::ptr&& request) {
        auto response = std::make_unique<http::response>();

        std::lock_guard<std::mutex> curl_lock(m_mutex);

        // setup the request
        m_curl.set_option<CURLOPT_URL>(request->m_url);
        m_curl.set_option<CURLOPT_WRITEDATA>(&response->m_data);
        m_curl.set_option<CURLOPT_HEADERDATA>(&response->m_header);

        // set the HTTP method
        if (request->m_method == http::request::method_get) {
            m_curl.set_option<CURLOPT_HTTPGET>(true);
            m_curl.set_option<CURLOPT_FOLLOWLOCATION>(true);
        } else if (request->m_method == http::request::method_post) {
            m_curl.set_option<CURLOPT_POST>(true);
            auto post_fields = make_post_fields_string(request);
            m_curl.set_option<CURLOPT_POSTFIELDSIZE>(post_fields.size());
            m_curl.set_option<CURLOPT_COPYPOSTFIELDS>(post_fields);
        } else if (request->m_method == http::request::method_head) {
            m_curl.set_option<CURLOPT_NOBODY>(true);
        } else if (request->m_method == http::request::method_put) {
            m_curl.set_option<CURLOPT_PUT>(true);
        } else if (request->m_method == http::request::method_delete) {
            m_curl.set_option<CURLOPT_CUSTOMREQUEST>("DELETE");
        }

        // set the request headers
        auto& headers = request->m_headers;
        if (!headers.empty()) {
            m_headers.clear();
            for (auto& header : headers) {
                m_headers.append(header);
            }
            m_curl.set_option<CURLOPT_HTTPHEADER>(m_headers);
        }

        // perform the request
        m_curl.perform();

        // get information about the request

        auto response_code = *m_curl.get_info<CURLINFO_RESPONSE_CODE>();
        response->m_status = static_cast<http::status_t>(response_code);

        if (m_curl.error_code() != CURLE_OK) {
            response->m_error = m_curl.error_msg();
        }

        auto& state = request->m_state;

        {
            std::lock_guard<std::mutex> state_lock(state->mutex);
            state->response = std::move(response);
            // invoke callback
            if (state->callback) {
                state->callback(*state->response);
            }
        }

        // notify blocked threads
        state->cv.notify_all();

        // reset some request specific options
        reset_curl_handle();
    }

    // callback function used by libcurl to collect response & header data
    static std::size_t write_callback(char* data, std::size_t size, std::size_t nmemb, void* user_data) {
        auto total_size = size * nmemb;
        auto buffer = static_cast<memory::data*>(user_data);
        auto ptr = std::realloc(buffer->bytes, buffer->size + total_size);
        if (!ptr) {
            return 0;
        }
        buffer->bytes = reinterpret_cast<std::byte*>(ptr);
        std::memcpy(&(buffer->bytes[buffer->size]), data, total_size);
        buffer->size += total_size;
        return total_size;
    }

    std::string make_post_fields_string(const request::ptr& request) {
        std::vector<std::string> post_fields;
        post_fields.reserve(request->m_post_fields.size());
        for (auto& [key, value] : request->m_post_fields) {
            auto encoded_value = m_curl.escape(value);
            auto entry = shard::fmt("%s=%s", key.c_str(), encoded_value.c_str());
            post_fields.push_back(entry);
        }
        return shard::join(post_fields, "&");
    }

private:
    curl::handle m_curl;
    curl::slist m_headers;
    std::mutex m_mutex;

    std::size_t m_timeout = 0;
    std::size_t m_connection_timeout = 0;

    shard::channel<request::ptr> m_requests;

    std::thread m_thread;
};

} // namespace shard::net::http

#endif // SHARD_HTTP_CLIENT_HPP
