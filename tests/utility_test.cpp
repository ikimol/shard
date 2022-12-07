// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/utility/defer.hpp>

#include <doctest.h>

TEST_CASE("utility") {
    SUBCASE("defer") {
        bool called = false;
        {
            auto _ = shard::defer([&] { called = true; });
        }
        REQUIRE(called);
    }
}