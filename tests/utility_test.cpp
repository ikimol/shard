// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/utility/defer.hpp>
#include <shard/utility/hash_value.hpp>

#include <doctest.h>

TEST_CASE("utility") {
    SUBCASE("defer") {
        bool called = false;
        {
            auto _ = shard::defer([&] { called = true; });
        }
        REQUIRE(called);

        called = false;
        {
            SHARD_DEFER() {
                called = true;
            };
        }
        REQUIRE(called);
    }

    SUBCASE("hashing") {
        auto i1 = shard::hash_value(1, 2, 3);
        auto i2 = shard::hash_value(1, 2, 3);
        REQUIRE(i1 == i2);

        auto str1 = shard::hash_value(std::string("foo"));
        auto str2 = shard::hash_value(std::string("FOO"));
        REQUIRE(str1 != str2);
    }
}