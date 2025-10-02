// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#include <shard/crypto/sha.hpp>

#include <iostream>

static auto g_string = "foobar";

int main(int /* argc */, char* /* argv */[]) {
    std::cout << shard::sha256_string(g_string) << '\n';
}
