// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/enums.hpp>

#include <doctest.h>

enum class side : unsigned int {
    left = 0x1,
    top = 0x2,
    right = 0x4,
    bottom = 0x8,
};
SHARD_DECLARE_FLAGS(margin, side);
SHARD_DECLARE_FLAG_OPERATORS(margin)

TEST_SUITE("enums") {
    TEST_CASE("flags") {
        margin m = (side::left | side::right);

        CHECK(m.test(side::left));
        CHECK(m.test(side::right));

        CHECK_FALSE(m.test(side::top));
        CHECK_FALSE(m.test(side::bottom));
    }

    TEST_CASE("operators") {
        using namespace shard::enums::operators;
        CHECK((side::left == 1));
        CHECK((4 == side::right));
        CHECK((side::top != 1));
        CHECK((1 != side::bottom));
    }

    TEST_CASE("traits") {
        CHECK(shard::to_underlying(side::left) == 1);
        CHECK(shard::to_underlying(side::top) == 2);
        CHECK(shard::to_underlying(side::right) == 4);
        CHECK(shard::to_underlying(side::bottom) == 8);
    }
}