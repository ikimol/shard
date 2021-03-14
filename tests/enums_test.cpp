// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/enums.hpp>

#include <catch.hpp>

enum class side : unsigned int {
    left = 0x1,
    top = 0x2,
    right = 0x4,
    bottom = 0x8,
};
SHARD_DECLARE_FLAGS(margin, side);
SHARD_DECLARE_FLAG_OPERATORS(margin)

TEST_CASE("enums") {
    SECTION("flags") {
        margin m = (side::left | side::right);

        REQUIRE(m.test(side::left));
        REQUIRE(m.test(side::right));

        CHECK_FALSE(m.test(side::top));
        CHECK_FALSE(m.test(side::bottom));
    }

    SECTION("operators") {
        using namespace shard::enums::operators;
        REQUIRE((side::left == 1));
        REQUIRE((4 == side::right));
        REQUIRE((side::top != 1));
        REQUIRE((1 != side::bottom));
    }

    SECTION("traits") {
        REQUIRE(shard::to_underlying(side::left) == 1);
        REQUIRE(shard::to_underlying(side::top) == 2);
        REQUIRE(shard::to_underlying(side::right) == 4);
        REQUIRE(shard::to_underlying(side::bottom) == 8);
    }
}