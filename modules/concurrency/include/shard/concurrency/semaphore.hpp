// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/system/platform.hpp>
#include <shard/utility/preprocessor.hpp>

#include <cassert>
#include <cstddef>

#if defined(SHARD_APPLE)
#include <mach/mach_types.h>
#elif defined(SHARD_UNIX)
#include <semaphore.h>
#endif

namespace shard {
namespace concurrency {

class semaphore {
public:
    /// Create a new semaphore with the given count
    explicit semaphore(std::ptrdiff_t count = 0);

    /// Deleted copy constructor
    semaphore(const semaphore& other) = delete;

    /// Destroy the semaphore
    ~semaphore();

    /// Deleted copy assignment operator
    semaphore& operator=(const semaphore& other) = delete;

    /// Decrement the internal counter or block
    bool acquire();

    /// Increment the internal counter and unblock acquirers
    void release(std::ptrdiff_t count = 1);

private:
#if defined(SHARD_WINDOWS)
    using handle_type = void*;
#elif defined(SHARD_MACOS)
    using handle_type = semaphore_t;
#elif defined(SHARD_UNIX)
    using handle_type = sem_t;
#endif

private:
    handle_type m_handle;
};

} // namespace concurrency

// bring symbols into parent namespace

using concurrency::semaphore;

} // namespace shard
