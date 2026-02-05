// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/bit.hpp>

#include <doctest.h>

TEST_CASE("bit") {
    SUBCASE("set") {
        auto n = 0b0'1001u; // 9

        shard::bit::set(n, 1u);
        REQUIRE(n == 11);

        shard::bit::set(n, 2u);
        REQUIRE(n == 15);
    }

    SUBCASE("unset") {
        auto n = 0b0'1001u; // 9

        shard::bit::unset(n, 0u);
        REQUIRE(n == 8);

        shard::bit::unset(n, 1u);
        REQUIRE(n == 8);
    }

    SUBCASE("toggle") {
        auto n = 0b0'1001u; // 9

        shard::bit::toggle(n, 2u);
        REQUIRE(n == 13);

        shard::bit::toggle(n, 0u);
        REQUIRE(n == 12);
    }

    SUBCASE("is_set") {
        auto n = 0b0'1001u; // 9

        REQUIRE(shard::bit::is_set(n, 0u));
        REQUIRE(shard::bit::is_set(n, 3u));
        REQUIRE_FALSE(shard::bit::is_set(n, 1u));
    }

    SUBCASE("strip_last") {
        auto n = 0b0'1001u; // 9

        shard::bit::strip_last(n);
        REQUIRE(n == 8);

        shard::bit::strip_last(n);
        REQUIRE(n == 0);
    }

    SUBCASE("lowest") {
        auto n = 0b0'1011u; // 11

        REQUIRE(shard::bit::lowest(n) == 1);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::lowest(n) == 2);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::lowest(n) == 8);
    }

    SUBCASE("popcount") {
        auto n = 0b0'1011u; // 11

        REQUIRE(shard::bit::popcount(n) == 3);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::popcount(n) == 2);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::popcount(n) == 1);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::popcount(n) == 0);
    }

    SUBCASE("countr_zero") {
        std::uint8_t n = 0b1000'1011u; // 139

        REQUIRE(shard::bit::popcount(n) == 4);

        REQUIRE(shard::bit::countr_zero(n) == 0);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::countr_zero(n) == 1);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::countr_zero(n) == 3);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::countr_zero(n) == 7);
        shard::bit::strip_last(n);

        REQUIRE(shard::bit::countr_zero(n) == 8);
        REQUIRE(n == 0);

        SUBCASE("different sizes") {
            REQUIRE(shard::bit::countr_zero(static_cast<std::uint8_t>(0)) == 8);
            REQUIRE(shard::bit::countr_zero(static_cast<std::uint16_t>(0)) == 16);
            REQUIRE(shard::bit::countr_zero(static_cast<std::uint32_t>(0)) == 32);
            REQUIRE(shard::bit::countr_zero(static_cast<std::uint64_t>(0)) == 64);
        }
    }
}
