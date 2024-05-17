// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include "shard/concurrency/semaphore.hpp"

#include <cerrno>

#include <semaphore.h>

namespace shard::concurrency {

semaphore::semaphore(std::ptrdiff_t count) {
    assert(count >= 0);
    auto r = sem_init(&m_handle, 0, static_cast<unsigned int>(count));
    assert(r == 0);
    SHARD_UNUSED(r);
}

semaphore::~semaphore() {
    sem_destroy(&m_handle);
}

bool semaphore::acquire() {
    int r;
    do {
        r = sem_wait(&m_handle);
    } while (r == -1 && errno == EINTR);
    return r == 0;
}

void semaphore::release(std::ptrdiff_t count) {
    while (count-- > 0) {
        while (sem_post(&m_handle) != 0)
            ;
    }
}

} // namespace shard::concurrency
