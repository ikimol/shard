// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/property.hpp>

#include <doctest.h>

#include <string>

TEST_CASE("property") {
    SUBCASE("constructors") {
        shard::property<int> p1 = 42;
        REQUIRE(p1.value() == 42);

        shard::property<std::string> p2 = [] {
            return "foobar";
        };
        REQUIRE(p2.value() == "foobar");
    }

    SUBCASE("assignment") {
        shard::property<int> p;
        p = 42;
        REQUIRE(p.value() == 42);
    }

    SUBCASE("dependencies") {
        shard::property<int> p1 = 3;
        shard::property<int> p2 = 5;
        shard::property<double> p3 = [&] {
            return p1 / double(p2);
        };

        REQUIRE(p3.value() == 0.6);
        p1 = 2;
        REQUIRE(p3.value() == 0.4);

        SUBCASE("assignment removes dependencies") {
            p3 = 3.14;
            p1 = 4;
            REQUIRE(p3.value() == 3.14);
        }
    }

    SUBCASE("observed property") {
        bool did_update = false;
        shard::observed_property<int> p = {[&](auto) { did_update = true; }, 0};
        REQUIRE(p.value() == 0);
        REQUIRE_FALSE(did_update);

        p = 42;
        REQUIRE(p.value() == 42);
        REQUIRE(did_update);
    }
}
