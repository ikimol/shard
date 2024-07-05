// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include <shard/sparse_set.hpp>

#include <cassert>
#include <cstdio>

int main(int /* argc */, char* /* argv */[]) {
    shard::sparse_set set;

    assert(set.is_empty());
    assert(!set.contains(0));

    set.insert(0);
    assert(set.contains(0));
    assert(set.size() == 1);

    set.insert(10);
    assert(set.capacity() == 16);
    assert(set.contains(10));

    set.erase(0);
    assert(!set.contains(0));

    set.insert(20);
    assert(set.capacity() == 32);
    assert(set.contains(20));

    for (auto v : set) {
        std::printf("%zu\n", v);
    }

    return 0;
}
