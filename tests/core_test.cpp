// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/core/byte.hpp>
#include <shard/core/defer.hpp>

#include <catch.hpp>

// fill the buffer with integers and return the number of bytes written
static std::size_t buffer_manipulator(unsigned char* buffer, std::size_t size) {
    auto count = size / sizeof(int);
    for (std::size_t i = 0; i < count; ++i) {
        buffer[i * sizeof(int)] = i;
    }
    return count * sizeof(int);
}

TEST_CASE("core") {
    SECTION("defer") {
        bool called = false;
        {
            auto _ = shard::defer([&] { called = true; });
        }
        REQUIRE(called);
    }

    SECTION("byte") {
        const auto b = shard::to_byte(2);
        REQUIRE(shard::to_integer<int>(b) == 2);

        const auto b1 = shard::to_byte(0b0011);
        const auto b2 = shard::to_byte(0b1010);

        SECTION("integer value") {
            REQUIRE(shard::to_integer<int>(b1) == 3);
            REQUIRE(shard::to_integer<int>(b2) == 10);
        }

        SECTION("binary operations") {
            REQUIRE(shard::to_integer<int>(b1 | b2) == 11);
            REQUIRE(shard::to_integer<int>(b1 & b2) == 2);
        }

        SECTION("usage as buffer") {
            shard::byte buffer[22];
            auto byte_out = buffer_manipulator((unsigned char*)(buffer), 22);
            REQUIRE(byte_out == 20);
            REQUIRE(shard::to_integer<int>(buffer[0 * sizeof(int)]) == 0);
            REQUIRE(shard::to_integer<int>(buffer[1 * sizeof(int)]) == 1);
            REQUIRE(shard::to_integer<int>(buffer[2 * sizeof(int)]) == 2);
            REQUIRE(shard::to_integer<int>(buffer[3 * sizeof(int)]) == 3);
            REQUIRE(shard::to_integer<int>(buffer[4 * sizeof(int)]) == 4);
        }
    }
}