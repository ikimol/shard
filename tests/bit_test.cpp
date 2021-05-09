// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/bit.hpp>

#include <catch.hpp>

TEST_CASE("bit") {
    SECTION("set") {
        auto n = 0b01001; // 9

        shard::bit::set(n, 1);
        REQUIRE(n == 11);

        shard::bit::set(n, 2);
        REQUIRE(n == 15);
    }

    SECTION("unset") {
        auto n = 0b01001; // 9

        shard::bit::unset(n, 0);
        REQUIRE(n == 8);

        shard::bit::unset(n, 1);
        REQUIRE(n == 8);
    }

    SECTION("toggle") {
        auto n = 0b01001; // 9

        shard::bit::toggle(n, 2);
        REQUIRE(n == 13);

        shard::bit::toggle(n, 0);
        REQUIRE(n == 12);
    }

    SECTION("is_set") {
        auto n = 0b01001; // 9

        REQUIRE(shard::bit::is_set(n, 0));
        REQUIRE(shard::bit::is_set(n, 3));
        REQUIRE_FALSE(shard::bit::is_set(n, 1));
    }

    SECTION("strip_last") {
        auto n = 0b01001; // 9

        shard::bit::strip_last(n);
        REQUIRE(n == 8);

        shard::bit::strip_last(n);
        REQUIRE(n == 0);
    }

    SECTION("lowest") {
        auto n = 0b01011; // 11

        REQUIRE(shard::bit::lowest(n) == 1);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::lowest(n) == 2);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::lowest(n) == 8);
    }

    SECTION("count") {
        auto n = 0b01011; // 11

        REQUIRE(shard::bit::count(n) == 3);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::count(n) == 2);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::count(n) == 1);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::count(n) == 0);
    }
}
