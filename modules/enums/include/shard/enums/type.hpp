// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/meta/type_traits.hpp>

namespace shard {
namespace enums {
namespace detail {

template <typename... Args>
using enable_if_enum_t = std::enable_if_t<are_enum_v<std::remove_reference_t<Args>...>>;

} // namespace detail

/// Cast an enum to its underlying type
template <typename E, typename T = std::underlying_type_t<E>, typename = detail::enable_if_enum_t<E>>
constexpr T to_underlying(E e) noexcept {
    return static_cast<T>(e);
}

/// Cast an optional enum to its underlying type
template <typename E>
constexpr auto to_underlying(const std::optional<E>& opt_e) -> std::optional<decltype(to_underlying(*opt_e))> {
    if (opt_e) {
        return to_underlying(*opt_e);
    }
    return std::nullopt;
}

/// Cast a raw value to an enum
template <typename E, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
constexpr E enum_cast(T raw_value) {
    auto value = static_cast<std::underlying_type_t<E>>(raw_value);
    return static_cast<E>(value);
}

} // namespace enums

// bring symbols into parent namespace

using enums::enum_cast;
using enums::to_underlying;

} // namespace shard