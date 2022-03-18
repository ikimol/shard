// Copyright (c) 2022 Miklos Molnar. All rights reserved.

#include <shard/property.hpp>

#include <catch.hpp>

#include <string>

TEST_CASE("property") {
    SECTION("constructors") {
        shard::property<int> p1 = 42;
        REQUIRE(p1.get() == 42);

        shard::property<std::string> p2 = [] {
            return "foobar";
        };
        REQUIRE(p2.get() == "foobar");
    }

    SECTION("assignment") {
        shard::property<int> p;
        p = 42;
        REQUIRE(p.get() == 42);
    }

    SECTION("dependencies") {
        shard::property<int> p1 = 3;
        shard::property<int> p2 = 5;
        shard::property<double> p3 = [&] {
            return p1 / double(p2);
        };

        REQUIRE(p3.get() == 0.6);
        p1 = 2;
        REQUIRE(p3.get() == 0.4);

        SECTION("assignment removes dependencies") {
            p3 = 3.14;
            p1 = 4;
            REQUIRE(p3.get() == 3.14);
        }
    }

    SECTION("observed property") {
        bool did_update = false;
        shard::observed_property<int> p = {[&] { did_update = true; }, 0};
        REQUIRE(p.get() == 0);
        REQUIRE_FALSE(did_update);

        p = 42;
        REQUIRE(p.get() == 42);
        REQUIRE(did_update);
    }
}
