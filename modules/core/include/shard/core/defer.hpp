// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_CORE_DEFER_HPP
#define SHARD_CORE_DEFER_HPP

#include <shard/core/non_copyable.hpp>

#include <utility>

namespace shard {
namespace core {

template <typename F>
class deferred_function : private core::non_copyable {
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

} // namespace core

// bring symbols into parent namespace

using core::defer;

} // namespace shard

#endif // SHARD_CORE_DEFER_HPP
