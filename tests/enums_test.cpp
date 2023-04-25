// Copyright (c) 2023 Miklos Molnar. All rights reserved.

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

TEST_CASE("enums") {
    SUBCASE("flags") {
        margin m = (side::left | side::right);

        REQUIRE(m.test(side::left));
        REQUIRE(m.test(side::right));

        REQUIRE_FALSE(m.test(side::top));
        REQUIRE_FALSE(m.test(side::bottom));
    }

    SUBCASE("operators") {
        using namespace shard::enums::operators;
        REQUIRE((side::left == 1));
        REQUIRE((4 == side::right));
        REQUIRE((side::top != 1));
        REQUIRE((1 != side::bottom));
    }

    SUBCASE("traits") {
        REQUIRE(shard::to_underlying(side::left) == 1);
        REQUIRE(shard::to_underlying(side::top) == 2);
        REQUIRE(shard::to_underlying(side::right) == 4);
        REQUIRE(shard::to_underlying(side::bottom) == 8);
    }

    SUBCASE("enum_set") {
        enum class side { left, top, right, bottom };
        using padding = shard::enum_set<side, 4>;

        padding sides;
        sides.set(side::left);
        sides.set(side::top);

        REQUIRE(sides.test(side::left));
        REQUIRE(sides.test(side::top));
        REQUIRE_FALSE(sides.test(side::right));
        REQUIRE_FALSE(sides.test(side::bottom));

        sides.set_all();
        REQUIRE(sides.all());

        SUBCASE("constructors") {
            padding p1;
            REQUIRE(p1.none());

            padding p2({side::left});
            REQUIRE(p2.test(side::left));

            padding p3(side::right, side::bottom);
            REQUIRE(p3.test(side::right));
            REQUIRE(p3.test(side::bottom));
        }

        SUBCASE("operations") {
            padding lhs;
            lhs.set(side::left);
            lhs.set(side::top);

            padding rhs;
            rhs.set(side::right);
            rhs.set(side::bottom);

            REQUIRE(lhs.union_with(rhs).all());
            REQUIRE(lhs.intersection_with(rhs).none());

            rhs.set(side::left);
            rhs.set(side::bottom, false);
            auto diff = rhs.subtracting(lhs);
            REQUIRE(diff.any());
            REQUIRE(diff == side::right);

            REQUIRE(diff.is_subset_of(rhs));
            REQUIRE_FALSE(diff.is_subset_of(lhs));
        }
    }
}