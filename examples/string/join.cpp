// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/string/join.hpp>

#include <iostream>
#include <set>
#include <vector>

int main(int /* argc */, char* /* argv */[]) {
    std::vector<const char*> v = {"foo", "bar", "baz"};
    std::set<int> s {1, 2, 3};

    std::cout << shard::join(v, " - ") << '\n';
    std::cout << shard::join(s, ".") << '\n';

    return 0;
}
