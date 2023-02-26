// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/containers/hash_map.hpp>
#include <shard/memory/allocators/heap_allocator.hpp>

#include <iostream>
#include <string>

const char* g_names[] {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

const char* remainder_name(std::size_t n) {
    return g_names[n % 10];
}

int main(int /* argc */, char* /* argv */[]) {
    shard::heap_allocator allocator;
    {
        shard::hash_map<int, std::string> m(allocator);
        auto bc = m.bucket_count();
        for (int i = 0; i < 50; ++i) {
            m.emplace(i, remainder_name(i));
            if (bc != m.bucket_count()) {
                bc = m.bucket_count();
                std::cout << "new bucket count: " << bc << '\n';
            }
        }
        for (int i = 10; i < 20; ++i) {
            std::cout << m[i] << ' ';
        }
        std::cout << '\n';
    }
    return 0;
}
