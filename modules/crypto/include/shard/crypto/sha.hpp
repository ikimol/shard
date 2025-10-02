// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/memory/data.hpp>
#include <shard/memory/dynamic_data.hpp>

#include <string>
#include <string_view>

namespace shard {
namespace crypto {

/// Encode a piece of data using SHA256
dynamic_data sha256(const data& data);

/// Encode a string using SHA256
inline dynamic_data sha256(std::string_view string) {
    return sha256(data(string.data(), string.length()));
}

/// Encode a piece of data using SHA256 into a string
std::string sha256_string(const data& data);

/// Encode a string using SHA256 into a string
inline std::string sha256_string(std::string_view string) {
    return sha256_string(data(string.data(), string.length()));
}

} // namespace crypto

// bring symbols into parent namespace

using crypto::sha256;
using crypto::sha256_string;

} // namespace shard