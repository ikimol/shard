// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_MATH_PRIME_HPP
#define SHARD_MATH_PRIME_HPP

#include <cstddef>

namespace shard {
namespace math {

/// Check whether a number is a prime number
inline bool is_prime(std::size_t n) {
    if (n <= 1) {
        return false;
    }

    if (n <= 3) {
        return true;
    }

    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }

    for (std::size_t i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }

    return true;
}

/// Get the next prime number greater than the given number
inline std::size_t next_prime(std::size_t n) {
    if (n <= 1) {
        return 2;
    }
    auto prime = n;
    bool found = false;
    while (!found) {
        ++prime;
        if (is_prime(prime)) {
            found = true;
        }
    }
    return prime;
}

} // namespace math

// bring symbols into parent namespace

using math::is_prime;
using math::next_prime;

} // namespace shard

#endif // SHARD_MATH_PRIME_HPP
