// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/math/degrees.hpp>

#include <iostream>

using namespace shard;

int main(int /* argc */, char* /* argv */[]) {
    std::cout << math::deg(M_PI) << '\n';
    std::cout << math::rad(180.0) << '\n';

    return 0;
}
