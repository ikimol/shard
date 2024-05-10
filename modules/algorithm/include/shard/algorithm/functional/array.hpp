// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <array>

namespace shard::algorighm::functiona {
namespace detail {

template <typename T, std::size_t N, typename UnaryPredicate, std::size_t... Is>
constexpr auto map_array(const std::array<T, N>& a, UnaryPredicate&& p, std::index_sequence<Is...>) {
    return std::array<std::invoke_result_t<UnaryPredicate, const T&>, N> {p(a[Is])...};
}

} // namespace detail

/// Transform an array of values using the predicate
///
/// \note This will only work when the result container is also an array.
template <typename T, std::size_t N, typename UnaryPredicate>
[[nodiscard]] auto map(const std::array<T, N>& a, UnaryPredicate&& p) {
    return detail::map_array(a, std::forward<UnaryPredicate>(p), std::make_index_sequence<N> {});
}

} // namespace shard::algorighm::functiona
