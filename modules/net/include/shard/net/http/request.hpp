// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/net/http/detail/shared_state.hpp"
#include "shard/net/url.hpp"

#include <shard/string/fmt.hpp>
#include <shard/string/join.hpp>
#include <shard/utility/preprocessor.hpp>

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace shard::net::http {

class client;

class request {
    friend class client;

public:
    using ptr = std::shared_ptr<request>;

    enum method_t {
        method_get,
        method_head,
        method_post,
        method_put,
        method_delete,
    };

public:
    /// Default constructor
    request() = default;

    /// Create a new HTTP request
    explicit request(net::url&& url, method_t method = method_get)
    : m_url(std::move(url))
    , m_method(method) {}

    /// Get the URL
    const net::url& url() const { return m_url; }

    /// Set the URL
    void set_url(net::url url) { m_url = std::move(url); }

    /// Get the HTTP request method
    method_t method() const { return m_method; }

    /// Set the HTTP request method
    void set_method(method_t method) { m_method = method; }

    /// Check if the request can be cancelled
    bool is_cancellable() const { return m_cancellable; }

    /// Make the request cancellable
    void make_cancellable() { m_cancellable = true; }

    /// Add a single HTTP header
    void set_header(const std::string& key, std::optional<std::string> value) {
        if (key.empty()) {
            return;
        }
        std::string entry;
        if (value) {
            entry = shard::fmt("%s: %s", key.c_str(), value->c_str());
        } else {
            entry = shard::fmt("%s;", key.c_str());
        }
        m_headers.insert_or_assign(key, std::move(entry));
    }

    /// Add a single post field
    template <typename T>
    void set_post_field(const std::string& key, T&& value) {
        if (key.empty()) {
            return;
        }
        m_post_fields.insert_or_assign(key, shard::to_string(std::forward<T>(value)));
    }

private:
    using header_map = std::unordered_map<std::string, std::string>;
    using post_fields = std::unordered_map<std::string, std::string>;

    // concatenate the post fields
    std::string merge_post_fields(const curl::handle& curl) {
        std::vector<std::string> post_fields;
        post_fields.reserve(m_post_fields.size());
        for (auto& [key, value] : m_post_fields) {
            auto encoded_value = curl.escape(value);
            auto entry = shard::fmt("%s=%s", key.c_str(), encoded_value.c_str());
            post_fields.push_back(entry);
        }
        return shard::join(post_fields, "&");
    }

    const header_map& headers() const { return m_headers; }

    detail::shared_state::ptr shared_state() const { return m_state; }

private:
    net::url m_url;
    method_t m_method = method_get;
    bool m_cancellable = false;

    header_map m_headers;
    post_fields m_post_fields;

    detail::shared_state::ptr m_state;
};

/// Factory class that builds a request
class request_builder {
public:
    /// Set the URL
    request_builder& with_url(net::url&& url) {
        m_request.set_url(std::move(url));
        return *this;
    }

    /// Set the HTTP request method
    request_builder& with_method(request::method_t method) {
        m_request.set_method(method);
        return *this;
    }

    /// Add a single HTTP header
    request_builder& with_header(const std::string& key, std::optional<std::string> value) {
        m_request.set_header(key, std::move(value));
        return *this;
    }

    /// Add a single post field
    template <typename T>
    request_builder& with_post_field(const std::string& key, T&& value) {
        m_request.set_post_field(key, std::forward<T>(value));
        return *this;
    }

    /// Make the request cancellable
    request_builder& make_cancellable() {
        m_request.make_cancellable();
        return *this;
    }

    /// Get the configured request
    request&& build() { return std::move(m_request); }

private:
    request m_request;
};

} // namespace shard::net::http
