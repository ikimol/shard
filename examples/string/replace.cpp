// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/string/replace.hpp>

#include <iostream>

int main(int /* argc */, char* /* argv */[]) {
    std::string s("foo bar baz quack");

    shard::replace_all(s, "o", "0");
    std::cout << s << '\n';

    auto copy = shard::replace_all_copy(s, "a", "@");
    std::cout << copy << '\n';

    return 0;
}
