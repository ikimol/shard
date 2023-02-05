// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_HTTP_REQUEST_HPP
#define SHARD_HTTP_REQUEST_HPP

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
    /// Create a new HTTP request
    static request::ptr create(url url, method_t method = method_get) {
        auto r = new request(std::move(url), method);
        return request::ptr {r};
    }

    /// Set the URL
    void set_url(url url) { m_url = std::move(url); }

    /// Set the HTTP request method
    void set_method(method_t method) { m_method = method; }

    /// Add a single HTTP header
    void set_header(const std::string& key, std::optional<std::string> value) {
        std::string line;
        if (value) {
            line = shard::fmt("%s: %s", key.c_str(), value->c_str());
        } else {
            line = shard::fmt("%s;", key.c_str());
        }
        m_headers.push_back(std::move(line));
    }

    /// Add a single post field
    template <typename T>
    void set_post_field(const std::string& key, T&& value) {
        if (key.empty()) {
            return;
        }
        m_post_fields[key] = shard::to_string(SHARD_FWD(value));
    }

private:
    // create an HTTP request with a URL and an HTTP method
    request(url&& url, method_t method)
    : m_url(std::move(url))
    , m_method(method) {}

private:
    url m_url;
    method_t m_method;

    std::vector<std::string> m_headers;
    std::unordered_map<std::string, std::string> m_post_fields;

    detail::shared_state::ptr m_state;
};

} // namespace shard::net::http

#endif // SHARD_HTTP_REQUEST_HPP
