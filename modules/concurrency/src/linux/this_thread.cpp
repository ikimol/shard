// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include "shard/concurrency/this_thread.hpp"

#include <cstring>

#include <pthread.h>

namespace shard::concurrency::this_thread {

void set_name(const char* name) {
    if (const auto length = std::strlen(name); length <= 15) {
        pthread_setname_np(pthread_self(), name);
    } else {
        char buffer[16];
        std::memcpy(buffer, name, 15);
        buffer[15] = '\0';
        pthread_setname_np(pthread_self(), buffer);
    }
}

} // namespace shard::concurrency::this_thread
