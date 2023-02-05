// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/uuid.hpp>

#include <doctest.h>

#include <vector>

TEST_CASE("uuid") {
    shard::uuid expected(
        {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef});

    SUBCASE("constructors") {
        SUBCASE("raw array") {
            std::uint8_t a[16] = {
                0x01,
                0x23,
                0x45,
                0x67,
                0x89,
                0xab,
                0xcd,
                0xef,
                0x01,
                0x23,
                0x45,
                0x67,
                0x89,
                0xab,
                0xcd,
                0xef,
            };

            shard::uuid id(a);
            REQUIRE_FALSE(id.is_null());
            REQUIRE(id == expected);
        }

        SUBCASE("iterators") {
            std::vector<std::uint8_t> v(16);
            v[0] = v[8] = 0x01;
            v[1] = v[9] = 0x23;
            v[2] = v[10] = 0x45;
            v[3] = v[11] = 0x67;
            v[4] = v[12] = 0x89;
            v[5] = v[13] = 0xab;
            v[6] = v[14] = 0xcd;
            v[7] = v[15] = 0xef;

            shard::uuid id(v.begin(), v.end());
            REQUIRE_FALSE(id.is_null());
            REQUIRE(id == expected);
        }
    }

    SUBCASE("string conversion") {
        auto id = shard::uuid::from_string("01234567-89ab-cdef-0123-456789abcdef");
        REQUIRE(id);
        auto s = id->to_string();
        REQUIRE(s == "01234567-89ab-cdef-0123-456789abcdef");
    }

    SUBCASE("system generator") {
        auto id = shard::uuid::make_system_uuid();
        REQUIRE_FALSE(id.is_null());
        REQUIRE(id.version() != shard::uuid::version::none);
    }
}
