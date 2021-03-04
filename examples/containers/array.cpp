// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/containers/array.hpp>
#include <shard/memory/allocators/heap_allocator.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    shard::heap_allocator allocator;
    {
        shard::array<int> a(allocator);
        auto c = a.capacity();
        for (int i = 0; i < 50; ++i) {
            a.emplace_back(i);
            if (c != a.capacity()) {
                c = a.capacity();
                std::cout << "new capacity: " << c << '\n';
            }
        }
        for (int i = 10; i < 20; ++i) {
            std::cout << a[i] << ' ';
        }
        std::cout << '\n';
    }
    return 0;
}