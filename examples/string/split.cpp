// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/string/split.hpp>

#include <iostream>
#include <vector>

int main(int /* argc */, char* /* argv */[]) {
    std::string s("foo,bar,baz,quack");

    std::vector<std::string> v;
    shard::split(s, std::back_inserter(v), ',');

    std::cout << v[0] << '\n';
    std::cout << v[1] << '\n';
    std::cout << v[2] << '\n';
    std::cout << v[3] << '\n';

    return 0;
}
