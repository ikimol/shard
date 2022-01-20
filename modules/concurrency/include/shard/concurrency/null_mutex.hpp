// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_CONCURRENCY_NULL_MUTEX_HPP
#define SHARD_CONCURRENCY_NULL_MUTEX_HPP

#include <shard/utility/non_copyable.hpp>

namespace shard {
namespace concurrency {

struct null_mutex : private utility::non_copyable {
    null_mutex() = default;

    null_mutex(null_mutex&&) = delete;
    null_mutex& operator=(null_mutex&&) = delete;

    void lock() /* NOLINT */ {}

    void unlock() /* NOLINT */ {}

    bool try_lock() /* NOLINT */ { return true; }
};

} // namespace concurrency

// bring symbols into parent namespace

using concurrency::null_mutex;

} // namespace shard

#endif // SHARD_CONCURRENCY_NULL_MUTEX_HPP
