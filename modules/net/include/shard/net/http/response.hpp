// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_HTTP_RESPONSE_HPP
#define SHARD_HTTP_RESPONSE_HPP

#include "shard/net/http/status.hpp"

#include <shard/enums/traits.hpp>
#include <shard/memory/data.hpp>

#include <optional>
#include <string>
#include <utility>

namespace shard::net::http {

class response {
    friend class client;

public:
    /// Get the HTTP status code
    status_t status() const { return m_status; }

    /// See if the request was successful by checking if its HTTP status code is
    /// in the 2xx range
    bool did_succeed() const { return to_underlying(m_status) >= 200 && to_underlying(m_status) < 300; }

    /// Get the curl error, if any
    const std::optional<std::string>& error() const { return m_error; }

    /// Get the response headers
    const memory::data& header() const { return m_header; }

    /// Get the response data
    const memory::data& data() const { return m_data; }

private:
    status_t m_status = status_t::unknown;

    std::optional<std::string> m_error;

    memory::data m_header;
    memory::data m_data;
};

} // namespace shard::net::http

#endif // SHARD_HTTP_RESPONSE_HPP
