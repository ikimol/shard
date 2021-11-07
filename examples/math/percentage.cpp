// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/math/percentage.hpp>

#include <iostream>

using namespace shard;

int main(int /* argc */, char* /* argv */[]) {
    math::percentage<int> p(2, 10);

    std::cout << p.from_value(12) << '\n';
    std::cout << p.to_value(0.25f) << '\n';
    std::cout << p.to_value(1.5f) << '\n';

    return 0;
}
