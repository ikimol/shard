// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/bit/traits.hpp"

#include <type_traits>

namespace shard::bit {

/// Set the bit in \a n at position \a p
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
void set(T& n, T p) {
    n |= (1 << p);
}

/// Unset the bit in \a n at position \a p
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
void unset(T& n, T p) {
    n &= (~(1 << p));
}

/// Toggle the bit in \a n at position \a p
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
void toggle(T& n, T p) {
    n ^= (1 << p);
}

/// Check if the bit in \a n at position \a p is set
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
bool is_set(T n, T p) {
    return (bool) (n & (1 << p));
}

/// Set the last bit set to 0
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
void strip_last(T& n) {
    n = n & (n - 1);
}

/// Get the value of the lowest bit set
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
T lowest(T n) {
    return n & (-n);
}

} // namespace shard::bit
