// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/net/curl/error.hpp"
#include "shard/net/http/response.hpp"

#include <shard/expected.hpp>

namespace shard::net::http {

/// The result of an HTTP request is either a response or an error
using result = shard::expected<response, curl::error>;

} // namespace shard::net::http
