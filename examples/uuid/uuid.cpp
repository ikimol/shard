// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/uuid.hpp>

#include <iostream>
#include <unordered_map>

int main(int /* argc */, char* /* argv */[]) {
    auto id = shard::uuid::make_system_uuid();

    std::cout << "uuid: " << id << '\n';
    std::cout << "variant: " << static_cast<int>(id.get_variant()) << '\n';
    std::cout << "version: " << static_cast<int>(id.get_version()) << '\n';

    std::unordered_map<shard::uuid, int> map;
    map[id] = 42;
    std::cout << "map[id]: " << map.at(id) << '\n';

    if (auto copy = shard::uuid::from_string(id.to_string())) {
        std::cout << "copy: " << *copy << '\n';
        std::cout << "map[copy]: " << map.at(*copy) << '\n';
    }

    {
        auto uuid = shard::uuid::make_v3_uuid(shard::uuid::namespace_dns, "foobar");
        std::cout << "uuid: " << uuid << " (version: " << static_cast<int>(uuid.get_version()) << "; DNS)\n";
    }

    {
        auto uuid = shard::uuid::make_v3_uuid(shard::uuid::namespace_url, "foobar");
        std::cout << "uuid: " << uuid << " (version: " << static_cast<int>(uuid.get_version()) << "; URL)\n";
    }

    {
        auto uuid = shard::uuid::make_v5_uuid(shard::uuid::namespace_dns, "foobar");
        std::cout << "uuid: " << uuid << " (version: " << static_cast<int>(uuid.get_version()) << ")\n";
    }

    return 0;
}
