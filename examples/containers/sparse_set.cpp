// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include <shard/sparse_set.hpp>

#include <cassert>
#include <cstdio>

namespace example {

struct widget_id {
    widget_id(std::size_t value) /* NOLINT */
    : value(value) {}

    explicit operator std::size_t() const { return value; }

    std::size_t value = 0;
};

} // namespace example

int main(int /* argc */, char* /* argv */[]) {
    shard::sparse_set<example::widget_id> set;

    assert(set.is_empty());
    assert(!set.contains(0));

    set.insert(0);
    assert(set.contains(0));
    assert(set.size() == 1);
    assert(set.index_of(0) == 0);

    set.insert(10);
    assert(set.capacity() == 16);
    assert(set.contains(10));
    assert(set.index_of(10) == 1);

    set.erase(0);
    assert(!set.contains(0));
    assert(set.index_of(10) == 0);

    set.insert(20);
    assert(set.capacity() == 32);
    assert(set.contains(20));
    assert(set.index_of(20) == 1);

    for (auto v : set) {
        std::printf("%zu\n", v.value);
    }

    return 0;
}
