// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/meta/type_traits.hpp"

#include <type_traits>
#include <utility>

namespace shard {

template <typename E>
class unexpected {
public:
    /// Copy constructor
    constexpr unexpected(const unexpected&) = default;

    /// Move constructor
    constexpr unexpected(unexpected&&) = default;

    template <typename Error = E,
              typename = std::enable_if_t<!std::is_same_v<unqualified_t<Error>, unexpected>
                                          && !std::is_same_v<unqualified_t<Error>, std::in_place_t>
                                          && std::is_constructible_v<E, Error>>>
    constexpr explicit unexpected(Error&& error) noexcept(std::is_nothrow_constructible_v<E, Error>)
    : m_error(std::forward<Error>(error)) {}

    template <typename... Args>
    constexpr explicit unexpected(std::in_place_t, Args&&... args) noexcept(std::is_nothrow_constructible_v<E, Args...>)
    : m_error(std::forward<Args>(args)...) {}

    constexpr unexpected& operator=(const unexpected&) = default;
    constexpr unexpected& operator=(unexpected&&) = default;

    constexpr const E& error() const noexcept { return m_error; }

    constexpr void swap(unexpected& other) noexcept(std::is_nothrow_swappable_v<E>) {
        static_assert(std::is_swappable_v<E>);
        using std::swap;
        swap(m_error, other.m_error);
    }

private:
    E m_error;
};

/// Deduction guide
template <typename E>
unexpected(E) -> unexpected<E>;

} // namespace shard
