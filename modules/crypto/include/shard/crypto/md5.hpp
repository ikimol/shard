// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/crypto/hash.hpp"

#include <string_view>

namespace shard {
namespace crypto {

/// Encode a piece of data using MD5
inline hash md5(const data& input) {
    return hash(hash::algorithm::md5, input);
}

/// Encode a string using MD5
inline hash md5(std::string_view string) {
    return md5(data(string.data(), string.length()));
}

} // namespace crypto

// bring symbols into parent namespace

using crypto::md5;

} // namespace shard
