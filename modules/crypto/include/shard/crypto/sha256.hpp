// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/crypto/hash.hpp>

#include <string_view>

namespace shard {
namespace crypto {

/// Encode a piece of data using SHA256
inline hash sha256(const data& input) {
    return hash(hash::algorithm::sha256, input);
}

/// Encode a string using SHA256
inline hash sha256(std::string_view string) {
    return sha256(data(string.data(), string.length()));
}

} // namespace crypto

// bring symbols into parent namespace

using crypto::sha256;

} // namespace shard
