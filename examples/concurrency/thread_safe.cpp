// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/concurrency/thread_safe.hpp>

#include <iostream>
#include <string>
#include <thread>
#include <vector>

using rw_string = shard::rw_thread_safe<std::string>;

static rw_string* g_string;

void thread_write(int id) {
    for (int i = 0; i < 10; ++i) {
        {
            shard::write_access<rw_string> string_w(*g_string);
            string_w->append(std::to_string(i) + ' ');
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));
    }
}

int main(int /* argc */, char* /* argv */[]) {
    rw_string string;
    g_string = &string;

    std::vector<std::thread> threads;
    threads.reserve(3);
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(thread_write, i);
    }

    std::cout << "threads launched\n";

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "threads joined\n";

    {
        shard::read_access<rw_string> string_r(*g_string);
        std::cout << *string_r << '\n';
    }

    return 0;
}
