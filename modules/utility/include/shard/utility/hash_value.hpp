// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <cstddef>
#include <functional>

namespace shard {
namespace utility {

/// Append the hash of multiple values to the seed
template <typename T, typename... Args>
void hash_combine(std::size_t& seed, const T& value, const Args&... args) {
    seed ^= std::hash<T> {}(value) + 0x9e3779b9 + (seed << 6u) + (seed >> 2u);
    (hash_combine(seed, args), ...);
}

/// Get the combined hash of multiple values
template <typename... Args>
std::size_t hash_value(const Args&... args) {
    std::size_t seed = 0;
    hash_combine(seed, args...);
    return seed;
}

} // namespace utility

// bring symbols into parent namespace

using utility::hash_value;

} // namespace shard
