// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_MATH_PERCENTAGE_HPP
#define SHARD_MATH_PERCENTAGE_HPP

#include "shard/math/utils.hpp"

namespace shard {
namespace math {

template <typename T>
class percentage {
public:
    /// Construct with start and end values
    percentage(T start, T end) : m_start(start), m_end(end) {}

    /// Get the percentage of the given value between start and end
    float from_value(T value) const { return pct(value, m_start, m_end); }

    /// Get the value at the given percentage
    T to_value(float p) { return m_start + p * (m_end - m_start); }

private:
    T m_start = 0;
    T m_end = 0;
};

} // namespace math
} // namespace shard

#endif // SHARD_MATH_PERCENTAGE_HPP
