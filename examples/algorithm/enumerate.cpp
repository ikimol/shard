// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/algorithm/enumerate.hpp>

#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

int main(int argc, char* argv[]) {
    std::list<int> list = {0, 1, 2, 3, 4};

    for (const auto& p : shard::enumerate(list)) {
        std::cout << "list[" << p.index() << "]: " << p.value() << '\n';
    }

    std::unordered_map<std::string, int> map = {{"zero", 0}, {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}};

    for (const auto& p : shard::enumerate(map)) {
        auto& pair = p.value();
        std::cout << '(' << p.index() << ") map['" << pair.first << "']: " << pair.second << '\n';
    }

    return 0;
}
