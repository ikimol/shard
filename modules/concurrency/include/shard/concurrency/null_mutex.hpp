// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

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
