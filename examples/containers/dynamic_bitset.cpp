// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include <shard/dynamic_bitset.hpp>

#include <bitset>
#include <cstdint>

int main(int /* argc */, char* /* argv */[]) {
    shard::dynamic_bitset<std::uint8_t> bits;
    bits.resize(1);
    bits.set(0, true);
    assert(bits.any());
    assert(bits.all());

    return 0;
}
