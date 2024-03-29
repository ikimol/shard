// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <type_traits>

namespace shard::bit {
namespace detail {

template <typename T>
using can_store_bits = std::bool_constant<std::is_integral<T>::value && std::is_unsigned<T>::value>;

} // namespace detail

/// Set the bit in \a n at position \a p
template <typename T, typename = std::enable_if_t<detail::can_store_bits<T>::value>>
void set(T& n, T p) {
    n |= (1 << p);
}

/// Unset the bit in \a n at position \a p
template <typename T, typename = std::enable_if_t<detail::can_store_bits<T>::value>>
void unset(T& n, T p) {
    n &= (~(1 << p));
}

/// Toggle the bit in \a n at position \a p
template <typename T, typename = std::enable_if_t<detail::can_store_bits<T>::value>>
void toggle(T& n, T p) {
    n ^= (1 << p);
}

/// Check if the bit in \a n at position \a p is set
template <typename T, typename = std::enable_if_t<detail::can_store_bits<T>::value>>
bool is_set(T n, T p) {
    return (bool)(n & (1 << p));
}

/// Set the last bit set to 0
template <typename T, typename = std::enable_if_t<detail::can_store_bits<T>::value>>
void strip_last(T& n) {
    n = n & (n - 1);
}

/// Get the value of the lowest bit set
template <typename T, typename = std::enable_if_t<detail::can_store_bits<T>::value>>
T lowest(T n) {
    return n & (-n);
}

/// Get the number of bits set
template <typename T, typename = std::enable_if_t<detail::can_store_bits<T>::value>>
unsigned int count(T n) {
    auto count = 0u;
    while (n) {
        n &= (n - 1);
        ++count;
    }
    return count;
}

} // namespace shard::bit
