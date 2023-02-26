// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <mutex>
#include <shared_mutex>

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

template <>
struct lock_traits<std::shared_lock> {
    static constexpr bool is_read_only = true;
};

} // namespace concurrency

// bring symbols into parent namespace

using concurrency::lock_traits;

} // namespace shard
