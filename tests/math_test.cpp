// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/math.hpp>

#include <doctest.h>

TEST_SUITE("math") {
    TEST_CASE("degrees") {
        CHECK(shard::deg(M_PI) == doctest::Approx(180.0));
        CHECK(shard::deg(M_PI / 2.0) == doctest::Approx(90.0));

        CHECK(shard::rad(180.0) == doctest::Approx(3.14159));
        CHECK(shard::rad(90.0) == doctest::Approx(1.570795));
    }

    TEST_CASE("percentage") {
        shard::math::percentage<float> p(2, 12);

        CHECK(p.from_value(7) == 0.5f);
        CHECK(p.from_value(9.5f) == 0.75f);
        CHECK(p.from_value(17.f) == 1.5f);

        CHECK(p.to_value(0.35f) == 5.5f);
        CHECK(p.to_value(1.2f) == 14.f);
    }

    TEST_CASE("utils") {
        CHECK(shard::min(0, 0) == 0);
        CHECK(shard::min(0, 5) == 0);
        CHECK(shard::min(0, -5) == -5);

        CHECK(shard::max(0, 0) == 0);
        CHECK(shard::max(0, 5) == 5);
        CHECK(shard::max(0, -5) == 0);

        CHECK(shard::clamp(5, 0, 10) == 5);
        CHECK(shard::clamp(-5, 0, 10) == 0);
        CHECK(shard::clamp(15, 0, 10) == 10);

        CHECK(shard::sq(1) == 1);
        CHECK(shard::sq(2) == 4);
        CHECK(shard::sq(3) == 9);

        CHECK(shard::pct(0, 0, 10) == 0.f);
        CHECK(shard::pct(5, 0, 10) == 0.5f);
        CHECK(shard::pct(10, 0, 10) == 1.f);
        CHECK(shard::pct(15, 0, 10) == 1.5f);
    }
}
