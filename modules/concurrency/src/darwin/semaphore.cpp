// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include "shard/concurrency/semaphore.hpp"

#include "shard/utility/preprocessor.hpp"

#include <cassert>

#include <mach/mach_init.h>
#include <mach/semaphore.h>
#include <mach/task.h>

namespace shard::concurrency {

semaphore::semaphore(std::ptrdiff_t count)
: m_handle(0) {
    assert(count >= 0);
    auto r = semaphore_create(mach_task_self(), &m_handle, SYNC_POLICY_FIFO, static_cast<int>(count));
    assert(r == KERN_SUCCESS);
    SHARD_UNUSED(r);
}

semaphore::~semaphore() {
    semaphore_destroy(mach_task_self(), m_handle);
}

bool semaphore::acquire() {
    return semaphore_wait(m_handle) == KERN_SUCCESS;
}

void semaphore::release(std::ptrdiff_t count) {
    while (count-- > 0) {
        while (semaphore_signal(m_handle) != KERN_SUCCESS)
            ;
    }
}

} // namespace shard::concurrency