// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

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
