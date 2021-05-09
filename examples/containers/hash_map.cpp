// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/containers/hash_map.hpp>
#include <shard/memory/allocators/heap_allocator.hpp>

#include <iostream>
#include <string>

const char* names[] {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

const char* remainder_name(std::size_t n) {
    return names[n % 10];
}

int main(int argc, char* argv[]) {
    shard::heap_allocator allocator;
    {
        shard::hash_map<int, std::string> a(allocator);
        auto bc = a.bucket_count();
        for (int i = 0; i < 50; ++i) {
            a.emplace(i, remainder_name(i));
            if (bc != a.bucket_count()) {
                bc = a.bucket_count();
                std::cout << "new bucket count: " << bc << '\n';
            }
        }
        for (int i = 10; i < 20; ++i) {
            std::cout << a[i] << ' ';
        }
        std::cout << '\n';
    }
    return 0;
}
