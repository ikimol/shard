// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include <shard/dynamic_bitset.hpp>

#include <cassert>
#include <cstdint>
#include <iostream>

int main(int /* argc */, char* /* argv */[]) {
    shard::dynamic_bitset<std::uint8_t> bits(8, 0b0000'1010);
    assert(bits.num_blocks() == 1);

    bits.resize(24, false);
    assert(bits.num_blocks() == 3);

    bits.push_back(true);
    assert(bits.num_blocks() == 4);

    bits.set(0, true);
    std::cout << "bits(" << bits.size() << ") = " << bits.to_string() << '\n';

    return 0;
}
