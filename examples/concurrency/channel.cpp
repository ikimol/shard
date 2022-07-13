// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/concurrency/channel.hpp>

#include <iostream>
#include <thread>

static shard::channel<int> g_channel;

static void thread_fn() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    g_channel.push(42);
}

int main(int /* argc */, char* /* argv */[]) {
    g_channel.push(1);
    g_channel.push(2);
    g_channel.push(3);

    for (int i = 0; i < 3; ++i) {
        // non-blocking read
        if (auto n = g_channel.try_pop()) {
            std::cout << *n << '\n';
        }
    }

    // launch a thread that will add an item to the channel after some delay
    std::thread t(thread_fn);

    int n = -1;
    g_channel.pop(n); // blocking read
    std::cout << n << '\n';

    t.join();

    return 0;
}