// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/memory/data.hpp>
#include <shard/memory/dynamic_data.hpp>

#include <string_view>

namespace shard {
namespace crypto {

/// Encode a piece of data as base64
dynamic_data base64_encode(const data& data);

/// Encode a string as base64
inline dynamic_data base64_encode(std::string_view string) {
    return base64_encode(data(string.data(), string.length()));
}

/// Decode a piece of base64 data
dynamic_data base64_decode(const data& data);

/// Decode a base64 string
inline dynamic_data base64_decode(std::string_view string) {
    return base64_decode(data(string.data(), string.length()));
}

} // namespace crypto

// bring symbols into parent namespace

using crypto::base64_decode;
using crypto::base64_encode;

} // namespace shard
