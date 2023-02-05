// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/uuid.hpp>

#include <iostream>
#include <unordered_map>

int main(int /* argc */, char* /* argv */[]) {
    auto id = shard::uuid::make_system_uuid();

    std::cout << "uuid: " << id << '\n';
    std::cout << "variant: " << (int)(id.variant()) << '\n';
    std::cout << "version: " << (int)(id.version()) << '\n';

    std::unordered_map<shard::uuid, int> map;
    map[id] = 42;
    std::cout << "map[id]: " << map.at(id) << '\n';

    if (auto copy = shard::uuid::from_string(id.to_string())) {
        std::cout << "copy: " << *copy << '\n';
        std::cout << "map[copy]: " << map.at(*copy) << '\n';
    }

    return 0;
}
