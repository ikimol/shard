// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/utility/defer.hpp>

#include <catch.hpp>

TEST_CASE("utility") {
    SECTION("defer") {
        bool called = false;
        {
            auto _ = shard::defer([&] { called = true; });
        }
        REQUIRE(called);
    }
}