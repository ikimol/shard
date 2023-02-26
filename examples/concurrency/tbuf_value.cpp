// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/concurrency/tbuf_value.hpp>
#include <shard/concurrency/thread_pool.hpp>

#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>

struct message {
    message() = default;

    explicit message(std::thread::id id, int value)
    : id(id)
    , value(value) {}

    std::thread::id id;
    int value = 0;
};

static std::atomic<bool> g_running = ATOMIC_VAR_INIT(true);
static std::atomic<int> g_counter = ATOMIC_VAR_INIT(0);
static shard::tbuf_value<message> g_value;

void producer() {
    while (g_running.load(std::memory_order_acquire)) {
        auto value = g_counter.fetch_add(1, std::memory_order_relaxed);
        g_value.store(message(std::this_thread::get_id(), value));
    }
}

void consumer() {
    int consumed = 0;
    std::unordered_map<std::thread::id, std::vector<decltype(g_counter)::value_type>> data;
    while (consumed < 30) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        auto message = g_value.load();
        data[message.id].emplace_back(message.value);
        ++consumed;
    }
    for (auto&& [id, values] : data) {
        std::cout << id << " (" << values.size() << "): ";
        for (auto value : values) {
            std::cout << value << ' ';
        }
        std::cout << '\n';
    }
    g_running.store(false, std::memory_order_release);
}

int main() {
    shard::thread_pool pool(4);
    pool.run(consumer);
    pool.run(producer);
    pool.run(producer);
    pool.run(producer);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return 0;
}
