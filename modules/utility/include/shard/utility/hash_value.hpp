// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_UTILITY_HASH_VALUE_HPP
#define SHARD_UTILITY_HASH_VALUE_HPP

#include <cstddef>
#include <utility>

namespace shard {
namespace utility {

template <typename T>
void hash_combine(std::size_t& seed, const T& value) {
    seed ^= std::hash<T> {}(value) + 0x9e3779b9 + (seed << 6u) + (seed >> 2u);
}

template <typename T, typename... Args>
void hash_combine(std::size_t& seed, const T& value, const Args&... args) {
    hash_combine(seed, value);
    hash_combine(seed, args...);
}

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

#endif // SHARD_UTILITY_HASH_VALUE_HPP
