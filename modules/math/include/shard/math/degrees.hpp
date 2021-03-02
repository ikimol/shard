// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_MATH_DEGREES_HPP
#define SHARD_MATH_DEGREES_HPP

#include <cmath>

namespace shard {
namespace math {

/// Convert degrees into radians
template <typename T>
constexpr T rad(T degrees) noexcept {
    return (M_PI / 180.0) * degrees;
}

/// Convert radians into degrees
template <typename T>
constexpr T deg(T radians) noexcept {
    return (180.0 / M_PI) * radians;
}

} // namespace math

// bring symbols into parent namespace

using math::deg;
using math::rad;

} // namespace shard

#endif // SHARD_MATH_DEGREES_HPP
