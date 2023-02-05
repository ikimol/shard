// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_UTILITY_TIMER_HPP
#define SHARD_UTILITY_TIMER_HPP

#include <chrono>

namespace shard {
namespace utility {

template <typename Clock>
class basic_timer {
public:
    using clock_type = Clock;
    using time_point = typename clock_type::time_point;
    using duration = typename clock_type::duration;

public:
    explicit basic_timer(duration& result)
    : m_result(&result)
    , m_start(clock_type::now()) {}

    ~basic_timer() {
        auto end = clock_type::now();
        *m_result = end - m_start;
    }

private:
    duration* m_result;
    time_point m_start;
};

using timer = basic_timer<std::chrono::high_resolution_clock>;

} // namespace utility

// bring symbols into parent namespace

using utility::timer;

} // namespace shard

#endif // SHARD_UTILITY_TIMER_HPP
