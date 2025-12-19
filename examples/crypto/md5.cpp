// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#include <shard/crypto/md5.hpp>

#include <iostream>

static auto g_string = "foobar";

int main(int /* argc */, char* /* argv */[]) {
    std::cout << shard::md5(g_string).to_string() << '\n';
}
