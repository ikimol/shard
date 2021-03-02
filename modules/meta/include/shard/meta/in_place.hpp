// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_META_IN_PLACE_HPP
#define SHARD_META_IN_PLACE_HPP

#include <cstddef>

namespace shard {
namespace meta {

// clang-format off
struct in_place_t { explicit in_place_t() = default; };
constexpr in_place_t in_place {};
// clang-format on

template <typename T> struct in_place_type_t { explicit in_place_type_t() = default; };
template <typename T> constexpr in_place_type_t<T> in_place_type {};

template <std::size_t> struct in_place_index_t { explicit in_place_index_t() = default; };
template <std::size_t I> constexpr in_place_index_t<I> in_place_index {};

} // namespace meta
} // namespace shard

#endif // SHARD_META_IN_PLACE_HPP
