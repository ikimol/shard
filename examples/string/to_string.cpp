// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/string/to_string.hpp>

#include <iostream>

struct widget {
    explicit widget(int id)
    : id(id) {}

    int id = 0;
};

static std::ostream& operator<<(std::ostream& os, const widget& w) {
    os << "widget(" << w.id << ')';
    return os;
}

int main(int /* argc */, char* /* argv */[]) {
    std::cout << shard::to_string(nullptr) << '\n';
    std::cout << shard::to_string('a') << '\n';
    std::cout << shard::to_string(false) << '\n';
    std::cout << shard::to_string("foo") << '\n';
    std::cout << shard::to_string(std::string("bar")) << '\n';
    std::cout << shard::to_string(42) << '\n';
    std::cout << shard::to_string(3.14f) << '\n';
    std::cout << shard::to_string(1.41) << '\n';
    std::cout << shard::to_string(widget {42}) << '\n';
    return 0;
}
