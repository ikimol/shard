// Copyright (c) 2022 Miklos Molnar. All rights reserved.

#ifndef SHARD_HTTP_REQUEST_HPP
#define SHARD_HTTP_REQUEST_HPP

#include "shard/net/http/detail/shared_state.hpp"
#include "shard/net/url.hpp"

#include <shard/string/fmt.hpp>
#include <shard/string/join.hpp>

#include <optional>

namespace shard::net::http {

class client;

class request : public std::enable_shared_from_this<request> {
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
    void set_post_field(const std::string& key, T&& value);

    /// Send this request
    future send();

private:
    explicit request(client& client) : m_client(client) {}

private:
    client& m_client;

    url m_url;
    method_t m_method = method_t::method_get;

    std::vector<std::string> m_headers;
    std::vector<std::string> m_post_fields;

    detail::shared_state::ptr m_state;
};

} // namespace shard::net::http

#endif // SHARD_HTTP_REQUEST_HPP
