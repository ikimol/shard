// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include <shard/concurrency/this_thread.hpp>

#include <thread>

static void thread_fn() {
    shard::this_thread::set_name("MyThread");
    // enter debugger to verify thread name
}

int main(int /* argc */, char* /* argv */[]) {
    std::thread(thread_fn).join();
    return 0;
}