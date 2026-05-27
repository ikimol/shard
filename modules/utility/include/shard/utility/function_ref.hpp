// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#pragma once

#include <functional>
#include <memory>
#include <type_traits>

namespace shard {
namespace utility {

template <typename F>
class function_ref;

/// A non-owning reference to a callable
template <typename R, typename... Args>
class function_ref<R(Args...)> {
private:
    template <typename F>
    static constexpr auto is_callable_v = std::bool_constant<!std::is_same_v<std::decay_t<F>, function_ref>
                                                             && std::is_invocable_r_v<R, F&&, Args...>>::value;

public:
    /// Default constructor
    constexpr function_ref() noexcept = delete;

    /// Copy constructor
    constexpr function_ref(const function_ref&) noexcept = default;

    /// Constructor from a callable
    template <typename F, typename = std::enable_if_t<is_callable_v<F>>>
    constexpr /* implicit */ function_ref(F&& f) noexcept /* NOLINT */
    : m_callable(reinterpret_cast<callable_type>(std::addressof(f))) {
        m_callback = [](callable_type callable, Args... args) -> R {
            return std::invoke(*reinterpret_cast<std::add_pointer_t<F>>(callable), std::forward<Args>(args)...);
        };
    }

    /// Copy assignment
    constexpr function_ref& operator=(const function_ref&) = default;

    /// Assignment from a callable
    template <typename F, typename = std::enable_if_t<is_callable_v<F>>>
    constexpr function_ref& operator=(F&& f) noexcept {
        m_callable = reinterpret_cast<callable_type>(std::addressof(f));
        m_callback = [](callable_type callable, Args... args) -> R {
            return std::invoke(*reinterpret_cast<std::add_pointer_t<F>>(callable), std::forward<Args>(args)...);
        };
        return *this;
    }

    /// Swap the callable with another
    constexpr void swap(function_ref& other) noexcept {
        std::swap(m_callable, other.m_callable);
        std::swap(m_callback, other.m_callback);
    }

    /// Call the stored callable with the given arguments.
    R operator()(Args... args) const { return m_callback(m_callable, std::forward<Args>(args)...); }

private:
    using callable_type = void (*)();
    using callback_type = R (*)(callable_type, Args...);

private:
    callable_type m_callable = nullptr;
    callback_type m_callback = nullptr;
};

template <typename R, typename... Args>
void swap(function_ref<R(Args...)>& lhs, function_ref<R(Args...)>& rhs) noexcept {
    lhs.swap(rhs);
}

// deduction guide

template <typename R, typename... Args>
function_ref(R (*)(Args...)) -> function_ref<R(Args...)>;

} // namespace utility

// bring symbols into parent namespace

using utility::function_ref;

} // namespace shard
