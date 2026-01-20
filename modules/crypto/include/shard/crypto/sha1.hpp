// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/crypto/hash.hpp"

#include <string_view>

namespace shard {
namespace crypto {

/// Encode a piece of data using SHA-1
inline hash sha1(const data& input) {
    return hash(hash::algorithm::sha1, input);
}

/// Encode a string using SHA-1
inline hash sha1(std::string_view string) {
    return sha1(data(string.data(), string.length()));
}

} // namespace crypto

// bring symbols into parent namespace

using crypto::sha1;

} // namespace shard
