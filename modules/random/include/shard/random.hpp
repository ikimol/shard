// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/meta/type_traits.hpp>

#include <cstdlib>
#include <random>
#include <utility>

namespace shard {

template <typename Distribution>
class random_base {
public:
    using engine_type = std::mt19937;
    using seed_type = engine_type::result_type;
    using dist_type = Distribution;
    using value_type = typename dist_type::result_type;
    using dist_param_type = typename dist_type::param_type;

public:
    template <typename... Args>
    explicit random_base(Args&&... args)
    : m_seed(engine_type::default_seed)
    , m_dist(std::forward<Args>(args)...) {}

    value_type next() { return m_dist(m_engine); }

    dist_param_type dist_param() const { return m_dist.param(); }

    void reseed() {
        std::random_device rd;
        reseed(rd());
    }

    void reseed(seed_type seed) {
        m_seed = seed;
        m_engine.seed(seed);
    }

    seed_type seed() const { return m_seed; }

private:
    engine_type m_engine;
    seed_type m_seed {};
    dist_type m_dist;
};

// clang-format off
template <typename T>
using random =
    std::conditional_t<meta::is_integer<T>::value,       // int
        random_base<std::uniform_int_distribution<T>>,
    std::conditional_t<std::is_floating_point<T>::value, // float
        random_base<std::uniform_real_distribution<T>>,
    std::conditional_t<meta::is_bool<T>::value,          // bool
        random_base<std::bernoulli_distribution>,
    void>>>;
// clang-format on

/// Seed the fast random number generator
inline void fast_random_seed(unsigned int seed) {
    std::srand(seed);
}

/// Fast random number generator
///
/// \details Using the C function 'rand()' is much faster, but has limited
/// randomness compared to the C++ alternative
inline int fast_random(int lower, int upper) {
    return (std::rand() % (upper - lower + 1)) + lower; /* NOLINT */
}

} // namespace shard
