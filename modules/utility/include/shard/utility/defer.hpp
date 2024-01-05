// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <utility>

namespace shard {
namespace utility {

template <typename F>
class deferred_function {
public:
    using function_type = F;

public:
    deferred_function() = delete;

    explicit deferred_function(function_type fn) noexcept
    : m_function(std::move(fn))
    , m_invoke(true) {}

    deferred_function(const deferred_function&) = delete;

    deferred_function(deferred_function&& other) noexcept(std::is_nothrow_move_constructible_v<F>)
    : m_function(std::move(other.m_function))
    , m_invoke(std::exchange(other.m_invoke, false)) {}

    ~deferred_function() noexcept {
        if (m_invoke) {
            m_function();
        }
    }

    deferred_function& operator=(const deferred_function&) = delete;
    deferred_function& operator=(deferred_function&&) = delete;

private:
    function_type m_function;
    bool m_invoke;
};

template <typename F>
deferred_function<F> defer(F&& func) noexcept {
    return deferred_function<F>(std::forward<F>(func));
}

} // namespace utility

// bring symbols into parent namespace

using utility::defer;

} // namespace shard
