// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/utility/byte.hpp>
#include <shard/utility/defer.hpp>

#include <catch.hpp>

// fill the buffer with integers and return the number of bytes written
static std::size_t buffer_manipulator(unsigned char* buffer, std::size_t size) {
    auto count = size / sizeof(int);
    for (std::size_t i = 0; i < count; ++i) {
        buffer[i * sizeof(int)] = i;
    }
    return count * sizeof(int);
}

TEST_CASE("utility") {
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

        SECTION("operators") {
            REQUIRE(shard::to_integer<int>(~b1) == 0b11111100);

            REQUIRE(shard::to_integer<int>(b1 | b2) == 0b1011);
            REQUIRE(shard::to_integer<int>(b1 & b2) == 0b0010);
            REQUIRE(shard::to_integer<int>(b1 ^ b2) == 0b1001);

            REQUIRE(shard::to_integer<int>(b1 << 1) == 0b0110);
            REQUIRE(shard::to_integer<int>(b2 >> 2) == 0b0010);
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