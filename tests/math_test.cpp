// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/math.hpp>

#include <catch.hpp>

TEST_CASE("math") {
    SECTION("degrees") {
        REQUIRE(shard::deg(M_PI) == Approx(180.0));
        REQUIRE(shard::deg(M_PI / 2.0) == Approx(90.0));

        REQUIRE(shard::rad(180.0) == Approx(3.14159));
        REQUIRE(shard::rad(90.0) == Approx(1.570795));
    }

    SECTION("percentage") {
        shard::math::percentage<float> p(2, 12);

        REQUIRE(p.from_value(7) == 0.5f);
        REQUIRE(p.from_value(9.5f) == 0.75f);
        REQUIRE(p.from_value(17.f) == 1.5f);

        REQUIRE(p.to_value(0.35f) == 5.5f);
        REQUIRE(p.to_value(1.2f) == 14.f);
    }

    SECTION("utils") {
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
