// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <type_traits>

namespace shard {

// expected

template <typename T, typename E>
class expected;

template <typename T>
struct is_expected : std::false_type {};

template <typename T, typename E>
struct is_expected<expected<T, E>> : std::true_type {};

template <typename T>
inline constexpr bool is_expected_v = is_expected<T>::value;

// unexpected

template <typename E>
class unexpected;

template <typename E>
struct is_unexpected : std::false_type {};

template <typename E>
struct is_unexpected<unexpected<E>> : std::true_type {};

template <typename T>
inline constexpr bool is_unexpected_v = is_unexpected<T>::value;

} // namespace shard
