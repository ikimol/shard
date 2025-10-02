// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/crypto/base64.hpp>

#include <iostream>

static auto g_string = R"(foobar_baz#quack123)";

int main(int /* argc */, char* /* argv */[]) {
    auto encoded = shard::base64_encode(g_string);
    std::cout << std::string_view(reinterpret_cast<char*>(encoded.bytes()), encoded.size()) << '\n';
    auto decoded = shard::base64_decode(encoded);
    std::cout << std::string_view(reinterpret_cast<char*>(decoded.bytes()), decoded.size()) << '\n';
    return 0;
}