// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_ENUMS_TRAITS_HPP
#define SHARD_ENUMS_TRAITS_HPP

#include <type_traits>

namespace shard {
namespace enums {
namespace detail {

template <typename... Args>
struct are_enum : std::true_type {};

template <typename T>
struct are_enum<T> : std::is_enum<T> {};

template <typename T, typename... Args>
struct are_enum<T, Args...> : std::integral_constant<bool, std::is_enum<T>::value && are_enum<Args...>::value> {};

template <typename... Args>
using enable_if_enum = std::enable_if_t<are_enum<std::remove_reference_t<Args>...>::value>;

} // namespace detail

/// Cast an enum to its underlying type
template <typename E, typename T = typename std::underlying_type<E>::type, typename = detail::enable_if_enum<E>>
constexpr T to_underlying(E e) noexcept {
    return static_cast<T>(e);
}

} // namespace enums

// bring symbols into parent namespace

using enums::to_underlying;

} // namespace shard

#endif // SHARD_ENUMS_TRAITS_HPP
