// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/containers/stack.hpp>
#include <shard/memory/allocators/heap_allocator.hpp>

#include <iostream>

int main(int /* argc */, char* /* argv */[]) {
    shard::heap_allocator allocator;
    {
        shard::stack<int> stack(allocator);
        stack.push(0);
        stack.push(1);
        stack.push(2);

        while (!stack.is_empty()) {
            std::cout << stack.top() << '\n';
            stack.pop();
        }
    }
    return 0;
}