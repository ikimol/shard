// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/net/http/status.hpp"

#include <shard/enums/traits.hpp>
#include <shard/memory/dynamic_data.hpp>

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
    bool is_successful() const { return to_underlying(m_status) >= 200 && to_underlying(m_status) < 300; }

    /// Get the response headers
    const memory::dynamic_data& header() const { return m_header; }

    /// Get the response data
    const memory::dynamic_data& data() const { return m_data; }

private:
    status_t m_status = status_t::unknown;

    memory::dynamic_data m_header;
    memory::dynamic_data m_data;
};

} // namespace shard::net::http
