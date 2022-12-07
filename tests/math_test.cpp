// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/math.hpp>

#include <doctest.h>

TEST_CASE("math") {
    SUBCASE("degrees") {
        REQUIRE(shard::deg(M_PI) == doctest::Approx(180.0));
        REQUIRE(shard::deg(M_PI / 2.0) == doctest::Approx(90.0));

        REQUIRE(shard::rad(180.0) == doctest::Approx(3.14159));
        REQUIRE(shard::rad(90.0) == doctest::Approx(1.570795));
    }

    SUBCASE("percentage") {
        shard::math::percentage<float> p(2, 12);

        REQUIRE(p.from_value(7) == 0.5f);
        REQUIRE(p.from_value(9.5f) == 0.75f);
        REQUIRE(p.from_value(17.f) == 1.5f);

        REQUIRE(p.to_value(0.35f) == 5.5f);
        REQUIRE(p.to_value(1.2f) == 14.f);
    }

    SUBCASE("prime") {
        SUBCASE("is_prime") {
            REQUIRE_FALSE(shard::is_prime(0));
            REQUIRE_FALSE(shard::is_prime(1));
            REQUIRE(shard::is_prime(2));
            REQUIRE(shard::is_prime(3));
            REQUIRE_FALSE(shard::is_prime(4));
            REQUIRE(shard::is_prime(5));
            REQUIRE_FALSE(shard::is_prime(6));
            REQUIRE(shard::is_prime(7));
            REQUIRE_FALSE(shard::is_prime(8));
            REQUIRE_FALSE(shard::is_prime(9));
            REQUIRE_FALSE(shard::is_prime(10));
            REQUIRE(shard::is_prime(11));
            REQUIRE_FALSE(shard::is_prime(12));
            REQUIRE(shard::is_prime(13));
            REQUIRE_FALSE(shard::is_prime(14));
            REQUIRE_FALSE(shard::is_prime(15));
            REQUIRE_FALSE(shard::is_prime(16));
            REQUIRE(shard::is_prime(17));
            REQUIRE_FALSE(shard::is_prime(18));
            REQUIRE(shard::is_prime(19));
        }

        SUBCASE("next_prime") {
            REQUIRE(shard::next_prime(0) == 2);
            REQUIRE(shard::next_prime(4) == 5);
            REQUIRE(shard::next_prime(10) == 11);
            REQUIRE(shard::next_prime(22) == 23);
            REQUIRE(shard::next_prime(46) == 47);
            REQUIRE(shard::next_prime(95) == 97);
            REQUIRE(shard::next_prime(194) == 197);
        }
    }

    SUBCASE("utils") {
        REQUIRE(shard::min(0, 0) == 0);
        REQUIRE(shard::min(0, 5) == 0);
        REQUIRE(shard::min(0, -5) == -5);

        REQUIRE(shard::max(0, 0) == 0);
        REQUIRE(shard::max(0, 5) == 5);
        REQUIRE(shard::max(0, -5) == 0);

        REQUIRE(shard::clamp(5, 0, 10) == 5);
        REQUIRE(shard::clamp(-5, 0, 10) == 0);
        REQUIRE(shard::clamp(15, 0, 10) == 10);

        REQUIRE(shard::sq(1) == 1);
        REQUIRE(shard::sq(2) == 4);
        REQUIRE(shard::sq(3) == 9);

        REQUIRE(shard::pct(0, 0, 10) == 0.f);
        REQUIRE(shard::pct(5, 0, 10) == 0.5f);
        REQUIRE(shard::pct(10, 0, 10) == 1.f);
        REQUIRE(shard::pct(15, 0, 10) == 1.5f);
    }
}
