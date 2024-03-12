// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include "shard/concurrency/semaphore.hpp"

#include <limits>

#include <windows.h>

namespace shard::concurrency {

semaphore::semaphore(std::ptrdiff_t count)
: m_handle(nullptr) {
    assert(count >= 0);
    auto max = std::numeric_limits<std::ptrdiff_t>::max();
    m_handle = CreateSemaphoreW(nullptr, count, max, nullptr);
    assert(m_handle);
}

semaphore::~semaphore() {
    CloseHandle(m_handle);
}

bool semaphore::acquire() {
    const unsigned long infinite = 0xffffffff;
    return WaitForSingleObject(m_handle, infinite) == 0;
}

void semaphore::release(std::ptrdiff_t count) {
    while (!ReleaseSemaphore(m_handle, count, nullptr))
        ;
}

} // namespace shard::concurrency
