// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/string/fmt.hpp>

#include <iostream>

int main(int /* argc */, char* argv[]) {
    std::cout << shard::fmt("%s, %d, %.2f", "foo", 42, 3.141592) << '\n';

    char buffer[32];
    std::cout << shard::sfmt(buffer, sizeof(buffer), "%p", (void*)(&argv)) << '\n';

    auto fmt = SHARD_MAKE_FMT(128, "value: %d");
    std::cout << fmt(0) << '\n';
    std::cout << fmt(42) << '\n';
    std::cout << fmt(8192) << '\n';

    return 0;
}
