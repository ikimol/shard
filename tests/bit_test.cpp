// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/bit.hpp>

#include <doctest.h>

TEST_CASE("bit") {
    SUBCASE("set") {
        auto n = 0b01001u; // 9

        shard::bit::set(n, 1u);
        REQUIRE(n == 11);

        shard::bit::set(n, 2u);
        REQUIRE(n == 15);
    }

    SUBCASE("unset") {
        auto n = 0b01001u; // 9

        shard::bit::unset(n, 0u);
        REQUIRE(n == 8);

        shard::bit::unset(n, 1u);
        REQUIRE(n == 8);
    }

    SUBCASE("toggle") {
        auto n = 0b01001u; // 9

        shard::bit::toggle(n, 2u);
        REQUIRE(n == 13);

        shard::bit::toggle(n, 0u);
        REQUIRE(n == 12);
    }

    SUBCASE("is_set") {
        auto n = 0b01001u; // 9

        REQUIRE(shard::bit::is_set(n, 0u));
        REQUIRE(shard::bit::is_set(n, 3u));
        REQUIRE_FALSE(shard::bit::is_set(n, 1u));
    }

    SUBCASE("strip_last") {
        auto n = 0b01001u; // 9

        shard::bit::strip_last(n);
        REQUIRE(n == 8);

        shard::bit::strip_last(n);
        REQUIRE(n == 0);
    }

    SUBCASE("lowest") {
        auto n = 0b01011u; // 11

        REQUIRE(shard::bit::lowest(n) == 1);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::lowest(n) == 2);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::lowest(n) == 8);
    }

    SUBCASE("count") {
        auto n = 0b01011u; // 11

        REQUIRE(shard::bit::count(n) == 3);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::count(n) == 2);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::count(n) == 1);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::count(n) == 0);
    }
}
