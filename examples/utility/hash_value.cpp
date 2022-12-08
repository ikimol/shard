// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/utility/hash_value.hpp>

#include <iostream>

int main(int /* argc */, char* /* argv */[]) {
    std::cout << shard::hash_value(false) << '\n';
    std::cout << shard::hash_value(std::string("bar")) << '\n';
    std::cout << shard::hash_value(42, 3.14f) << '\n';
    return 0;
}