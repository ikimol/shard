// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_UTILITY_DEFER_HPP
#define SHARD_UTILITY_DEFER_HPP

#include "shard/utility/non_copyable.hpp"

#include <utility>

namespace shard {
namespace utility {

template <typename F>
class deferred_function : private utility::non_copyable {
public:
    using function_type = F;

public:
    explicit deferred_function(function_type fn) noexcept : m_function(std::move(fn)) {}

    deferred_function(deferred_function&& other) noexcept :
    m_function(std::move(other.m_function)),
    m_invoke(std::exchange(other.m_invoke, false)) {}

    deferred_function& operator=(deferred_function&&) = delete;

    ~deferred_function() noexcept {
        if (m_invoke) {
            m_function();
        }
    }

private:
    function_type m_function;
    bool m_invoke = true;
};

template <typename F>
deferred_function<F> defer(F&& func) noexcept {
    return deferred_function<F>(std::forward<F>(func));
}

} // namespace utility

// bring symbols into parent namespace

using utility::defer;

} // namespace shard

#endif // SHARD_UTILITY_DEFER_HPP
