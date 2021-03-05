// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_CONCURRENCY_LOCK_TRAITS_HPP
#define SHARD_CONCURRENCY_LOCK_TRAITS_HPP

#include <mutex>

#if __cplusplus >= 201402L
#include <shared_mutex>
#endif

namespace shard {
namespace concurrency {

template <template <typename> class Lock>
struct lock_traits {};

template <>
struct lock_traits<std::lock_guard> {
    static constexpr bool is_read_only = false;
};

template <>
struct lock_traits<std::unique_lock> {
    static constexpr bool is_read_only = false;
};

#if __cplusplus >= 201402L
template <>
struct lock_traits<std::shared_lock> {
    static constexpr bool is_read_only = true;
};
#endif

} // namespace concurrency

// bring symbols into parent namespace

using concurrency::lock_traits;

} // namespace shard

#endif // SHARD_CONCURRENCY_LOCK_TRAITS_HPP
