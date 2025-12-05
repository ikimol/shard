// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#include <shard/hash_value.hpp>
#include <shard/version.hpp>

#include <doctest.h>

#include <unordered_set>

TEST_CASE("common") {
    SUBCASE("hash_value") {
        auto i1 = shard::hash_value(1, 2, 3);
        auto i2 = shard::hash_value(1, 2, 3);
        REQUIRE(i1 == i2);

        auto str1 = shard::hash_value(std::string("foo"));
        auto str2 = shard::hash_value(std::string("FOO"));
        REQUIRE(str1 != str2);
    }

    SUBCASE("version") {
        SUBCASE("parsing") {
            auto vs1 = shard::version::from_string("1.2.3");
            REQUIRE(vs1.has_value());
            REQUIRE(vs1 == shard::version(1, 2, 3));

            auto vs2 = shard::version::from_string("1.2");
            REQUIRE(vs2.has_value());
            REQUIRE(vs2 == shard::version(1, 2, 0));

            auto vs3 = shard::version::from_string("1");
            REQUIRE(vs3.has_value());
            REQUIRE(vs3 == shard::version(1, 0, 0));

            auto vs4 = shard::version::from_string("");
            REQUIRE_FALSE(vs4.has_value());

            auto vs5 = shard::version::from_string("a.b.c");
            REQUIRE_FALSE(vs5.has_value());
        }

        SUBCASE("to_string") {
            auto v = shard::version(1, 2, 3);
            REQUIRE(v.to_string() == "1.2.3");
        }

        SUBCASE("comparison") {
            auto v1 = shard::version(1, 1, 1);
            auto v2 = shard::version(1, 1, 0);
            REQUIRE(v2 < v1);

            auto v3 = shard::version(1, 0, 0);
            REQUIRE(v3 < v2);

            auto v4 = shard::version(0, 0, 0);
            REQUIRE(v4 < v3);
        }

        SUBCASE("hash") {
            auto v = shard::version(1, 2, 3);
            std::unordered_set<shard::version> versions;
            versions.insert(v);
            auto it = versions.find(shard::version(1, 2, 3));
            REQUIRE(it != versions.end());
        }
    }
}
