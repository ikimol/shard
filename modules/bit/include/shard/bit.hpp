// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_BIT_HPP
#define SHARD_BIT_HPP

#include <type_traits>

namespace shard {
namespace bit {

/// Set the bit in \a n at position \a p
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
void set(T& n, T p) {
    n |= (1 << p);
}

/// Unset the bit in \a n at position \a p
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
void unset(T& n, T p) {
    n &= (~(1 << p));
}

/// Toggle the bit in \a n at position \a p
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
void toggle(T& n, T p) {
    n ^= (1 << p);
}

/// Check if the bit in \a n at position \a p is set
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
bool is_set(T n, T p) {
    return (bool)(n & (1 << p));
}

/// Set the last bit set to 0
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
void strip_last(T& n) {
    n = n & (n - 1);
}

/// Get the value of the lowest bit set
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
T lowest(T n) {
    return n & (-n);
}

/// Get the number of bits set
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
unsigned int count(T n) {
    auto count = 0u;
    while (n) {
        n &= (n - 1);
        ++count;
    }
    return count;
}

} // namespace bit
} // namespace shard

#endif // SHARD_BIT_HPP
