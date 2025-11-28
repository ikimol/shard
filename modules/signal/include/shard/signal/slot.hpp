// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include <functional>
#include <type_traits>

namespace shard {
namespace detail {

class slot_base {
public:
    /// Check if the slot is enabled
    bool is_enabled() const { return m_enabled; }

    /// Set if the slot is enabled
    void set_enabled(bool enabled) { m_enabled = enabled; }

private:
    bool m_enabled = true;
};

} // namespace detail

/// Represents a callable connected to a slot
template <typename... Args>
class slot final : public detail::slot_base {
private:
    constexpr static auto has_args = sizeof...(Args) != 0;

public:
    using function_type = std::function<void(Args...)>;

public:
    /// Constructor with a convertible callable
    template <typename Function, typename = std::enable_if_t<std::is_constructible_v<function_type, Function>>>
    explicit slot(Function&& fn)
    : m_function(std::forward<Function>(fn)) {}

    /// Constructor with callable taking no arguments
    template <typename Function, typename = std::enable_if_t<has_args && std::is_invocable_v<Function>>>
    explicit slot(Function fn)
    : m_function([fn = std::move(fn)](Args&&...) { fn(); }) {}

    /// Invoke the stored callable
    template <typename... CallArgs>
    void invoke(CallArgs&&... args) {
        if (is_enabled()) {
            m_function(std::forward<CallArgs>(args)...);
        }
    }

private:
    function_type m_function;
};

} // namespace shard
