// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/net/curl/handle.hpp"
#include "shard/net/curl/http_version.hpp"
#include "shard/net/curl/proxy.hpp"
#include "shard/net/http/future.hpp"
#include "shard/net/http/request.hpp"
#include "shard/net/http/response.hpp"

#include <shard/concurrency/channel.hpp>

#include <chrono>
#include <cstddef>
#include <mutex>
#include <thread>

namespace shard::net::http {

class client {
public:
    /// Default constructor that starts the worker thread
    explicit client(curl::http_version_t version = curl::http_version_t::v_2)
    : m_timeout(std::chrono::milliseconds(0))
    , m_connection_timeout(std::chrono::milliseconds(0))
    , m_thread(&client::worker_thread, this) {
        setup_curl_handle(version);
    }

    /// Destructor that joins the worker thread
    ~client() {
        m_requests.close();
        m_thread.join();
    }

    /// Get the request timeout
    std::chrono::milliseconds timeout() const { return m_timeout; }

    /// Set the request timeout
    void set_timeout(std::chrono::milliseconds value) { m_timeout = value; }

    /// Get the connection timeout
    std::chrono::milliseconds connection_timeout() const { return m_connection_timeout; }

    /// Set the connection timeout
    void set_connection_timeout(std::chrono::milliseconds value) { m_connection_timeout = value; }

    /// Setup the proxy configuration
    void setup_proxy(std::string host, std::size_t port = 0, curl::proxy_t type = curl::proxy_t::http) {
        proxy p;
        p.host = std::move(host);
        p.port = port;
        p.type = type;
        m_proxy = std::move(p);
    }

    /// Disable the proxy
    void disable_proxy() { m_proxy.reset(); }

    /// Send the HTTP request
    ///
    /// \note This does not guarantee that the request is sent immediately.
    ///       The request is placed in a queue that is processed by the
    ///       background thread(s).
    ///
    /// \return A future object to add a callback to the request or block until
    ///         the response is available or the request times out.
    [[nodiscard]] future send_request(request&& request) {
        auto state = std::make_shared<detail::shared_state>();
        request.m_state = state;
        m_requests.push(std::move(request));
        return future(std::move(state));
    }

private:
    bool setup_curl_handle(curl::http_version_t version) {
        std::lock_guard curl_lock(m_mutex);

        bool result = m_curl.set_option<CURLOPT_HTTP_VERSION>(version);
        result = result && m_curl.set_option<CURLOPT_WRITEFUNCTION>(&client::write_callback);
        result = result && m_curl.set_option<CURLOPT_HEADERFUNCTION>(&client::write_callback);
        result = result && m_curl.set_option<CURLOPT_XFERINFOFUNCTION>(&client::progress_callback);

        return result;
    }

    void reset_curl_handle() {
        std::lock_guard curl_lock(m_mutex);

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
        m_curl.set_option<CURLOPT_XFERINFODATA>(nullptr);
        m_curl.set_option<CURLOPT_NOPROGRESS>(true);
    }

    void worker_thread() {
        while (auto request = m_requests.pop()) {
            perform_request(std::move(*request));
        }
    }

    void perform_request(request&& request) {
        auto state = request.shared_state();

        http::response response;

        std::lock_guard curl_lock(m_mutex);

        // setup the request
        bool setup_result = m_curl.set_option<CURLOPT_URL>(request.url());
        setup_result = setup_result && m_curl.set_option<CURLOPT_WRITEDATA>(&response.m_data);
        setup_result = setup_result && m_curl.set_option<CURLOPT_HEADERDATA>(&response.m_header);
        setup_result = setup_result && m_curl.set_option<CURLOPT_TIMEOUT_MS>(m_timeout.count());
        setup_result = setup_result && m_curl.set_option<CURLOPT_CONNECTTIMEOUT_MS>(m_connection_timeout.count());

        // setup the proxy
        if (m_proxy.has_value()) {
            setup_result = setup_result && m_curl.set_option<CURLOPT_PROXY>(m_proxy->host);
            setup_result = setup_result && m_curl.set_option<CURLOPT_PROXYPORT>(m_proxy->port);
            setup_result = setup_result && m_curl.set_option<CURLOPT_PROXYTYPE>(m_proxy->type);
        } else {
            setup_result = setup_result && m_curl.set_option<CURLOPT_PROXY>(std::nullopt);
        }

        // cancellation
        if (request.is_cancellable()) {
            setup_result = setup_result && m_curl.set_option<CURLOPT_XFERINFODATA>(state.get());
            setup_result = setup_result && m_curl.set_option<CURLOPT_NOPROGRESS>(false);
        }

        if (!setup_result) {
            return;
        }

        // set the HTTP method
        switch (request.method()) {
        case request::method_get: {
            m_curl.set_option<CURLOPT_HTTPGET>(true);
            m_curl.set_option<CURLOPT_FOLLOWLOCATION>(true);
        } break;
        case request::method_head: {
            m_curl.set_option<CURLOPT_NOBODY>(true);
        } break;
        case request::method_post: {
            m_curl.set_option<CURLOPT_POST>(true);
            auto post_fields = request.merge_post_fields(m_curl);
            m_curl.set_option<CURLOPT_POSTFIELDSIZE>(post_fields.size());
            m_curl.set_option<CURLOPT_COPYPOSTFIELDS>(post_fields);
        } break;
        case request::method_put: {
            m_curl.set_option<CURLOPT_PUT>(true);
        } break;
        case request::method_delete: {
            m_curl.set_option<CURLOPT_CUSTOMREQUEST>("DELETE");
        } break;
        }

        // set the request headers
        curl::slist http_header;
        if (auto& headers = request.headers(); !headers.empty()) {
            for (auto& [_, header] : headers) {
                http_header.append(header);
            }
            m_curl.set_option<CURLOPT_HTTPHEADER>(http_header);
        }

        // perform the request
        m_curl.perform();

        http::result result;

        if (m_curl.error_code() != CURLE_OK) {
            result = shard::unexpected(curl::error(m_curl.error_code()));
        } else if (auto status_code = m_curl.get_info<CURLINFO_RESPONSE_CODE>()) {
            response.m_status = static_cast<http::status_t>(*status_code);
            result = std::move(response);
        } else { // this should never be the case
            result = shard::unexpected(curl::error(CURLE_OK));
        }

        {
            std::lock_guard state_lock(state->mutex);
            state->is_available = true;
            state->result = std::move(result);
            state->invoke_callback();
        }

        // notify blocked threads
        state->cv.notify_all();

        // reset some request specific options
        reset_curl_handle();
    }

    // callback function used by libcurl to collect response & header data
    static std::size_t write_callback(char* data, std::size_t size, std::size_t nmemb, void* user_data) {
        auto total_size = size * nmemb;
        auto& buffer = *static_cast<memory::dynamic_data*>(user_data);
        auto current_size = buffer.size();
        if (auto new_bytes = buffer.reallocate(current_size + total_size)) {
            std::memcpy(&new_bytes[current_size], data, total_size); // write data
            return total_size;
        }
        return 0;
    }

    // callback function used by libcurl to report the progress of requests
    static int progress_callback(void* user_data,
                                 curl_off_t /* dl_total */,
                                 curl_off_t /* dl_current */,
                                 curl_off_t /* ul_total */,
                                 curl_off_t /* ul_current */) {
        auto state = static_cast<detail::shared_state*>(user_data);
        std::lock_guard lock(state->mutex);
        return state->is_cancelled ? 1 : 0;
    }

private:
    // group proxy options
    struct proxy {
        std::string host;
        std::size_t port = 0;
        curl::proxy_t type = curl::proxy_t::http;
    };

private:
    curl::handle m_curl;
    std::recursive_mutex m_mutex;

    std::chrono::milliseconds m_timeout;
    std::chrono::milliseconds m_connection_timeout;

    std::optional<proxy> m_proxy;

    shard::channel<request> m_requests;

    std::thread m_thread;
};

} // namespace shard::net::http
