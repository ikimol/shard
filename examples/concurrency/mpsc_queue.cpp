// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/concurrency/mpsc_queue.hpp>

#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <vector>

struct widget {
    explicit widget(std::uint64_t value)
    : value(value) {}
    std::uint64_t value;
};

static shard::mpsc_queue<widget, 256> g_queue;
static std::atomic<bool> g_exit = false;
static std::atomic<unsigned int> g_value = 0;
static std::mutex g_cout_mutex;

template <typename T>
void print_as_hex(std::ostream& os, const T& value) {
    std::ios::fmtflags flags(os.flags());
    os.setf(std::ios::hex);
    os << value;
    os.flags(flags);
}

void producer() {
    while (!g_exit) {
        auto pushed = g_queue.emplace(g_value.load());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (pushed) {
            {
                std::lock_guard<std::mutex> lock(g_cout_mutex);
                std::cout << '[';
                print_as_hex(std::cout, std::this_thread::get_id());
                std::cout << "] produced: " << g_value << '\n';
            }
            ++g_value;
        }
    }
}

void consumer() {
    while (!g_exit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (!g_queue.is_empty()) {
            auto t = g_queue.pop();
            if (t.has_value()) {
                auto value = t.value().value;
                {
                    std::lock_guard<std::mutex> lock(g_cout_mutex);
                    std::cout << "consumed: " << value << '\n';
                }
            }
        }
    }
}

int main() {
    std::vector<std::thread> threads;
    for (auto i = 0; i < 5; ++i) {
        threads.emplace_back(producer);
    }

    consumer();

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
