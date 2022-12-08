// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_MATH_UTILS_HPP
#define SHARD_MATH_UTILS_HPP

namespace shard {
namespace math {

/// Get the minimum of the two values
template <typename T>
constexpr T min(T a, T b) noexcept {
    return a < b ? a : b;
}

/// Get the maximum of the two values
template <typename T>
constexpr T max(T a, T b) noexcept {
    return a > b ? a : b;
}

/// Clamp a value between 'low' and 'high'
template <typename T>
constexpr T clamp(T x, T low, T high) noexcept {
    return max(low, min(x, high));
}

/// Return the square of the number
template <typename T>
constexpr T sq(T x) noexcept {
    return x * x;
}

/// Return the percentage value of 'x' between 'min' and 'max'
///
/// \example pct(8, 5, 10) == 0.6
template <typename T>
constexpr float pct(T x, T min, T max) noexcept {
    return (x - min) / static_cast<float>(max - min);
}

} // namespace math

// bring symbols into parent namespace

using math::clamp;
using math::max;
using math::min;
using math::pct;
using math::sq;

} // namespace shard

#endif // SHARD_MATH_UTILS_HPP
