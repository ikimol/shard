// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/uuid.hpp>

#include <doctest.h>

#include <vector>

TEST_CASE("uuid") {
    shard::uuid expected(
        {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef});

    SUBCASE("constructors") {
        SUBCASE("raw array") {
            std::uint8_t a[16]
                = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};

            shard::uuid id(a);
            REQUIRE_FALSE(id.is_nil());
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
            REQUIRE_FALSE(id.is_nil());
            REQUIRE(id == expected);
        }
    }

    SUBCASE("string conversion") {
        auto id = shard::uuid::from_string("01234567-89ab-cdef-0123-456789abcdef");
        REQUIRE(id);
        auto s = id->to_string();
        REQUIRE(s == "01234567-89ab-cdef-0123-456789abcdef");
    }

    SUBCASE("predefined") {
        REQUIRE(shard::uuid::nil.is_nil());

        auto bytes = shard::uuid::max.bytes();
        auto all_one = std::all_of(bytes, bytes + 16, [](auto b) { return b == 0xff; });
        REQUIRE(all_one);
    }

    SUBCASE("system generator") {
        auto id = shard::uuid::make_system_uuid();
        REQUIRE_FALSE(id.is_nil());
        REQUIRE(id.get_version() != shard::uuid::version::none);
    }

    SUBCASE("name based generator (v3)") {
        auto id = shard::uuid::make_v3_uuid(shard::uuid::namespace_dns, "foobar");
        REQUIRE_FALSE(id.is_nil());
        REQUIRE(id.get_version() == shard::uuid::version::name_based_md5);
    }

    SUBCASE("name based generator (v5)") {
        auto id = shard::uuid::make_v5_uuid(shard::uuid::namespace_dns, "foobar");
        REQUIRE_FALSE(id.is_nil());
        REQUIRE(id.get_version() == shard::uuid::version::name_based_sha1);
    }

    SUBCASE("predefined namespaces") {
        auto dns_id = shard::uuid(
            {0x43, 0x15, 0x99, 0x02, 0x71, 0x71, 0x30, 0xcf, 0xab, 0x7e, 0xee, 0x36, 0x98, 0x6c, 0x37, 0xc6});
        REQUIRE(shard::uuid::make_v3_uuid(shard::uuid::namespace_dns, "foobar") == dns_id);

        auto url_id = shard::uuid(
            {0x9e, 0xc8, 0x84, 0x48, 0x09, 0x4d, 0x30, 0xc8, 0x87, 0x51, 0xb3, 0x3d, 0x77, 0x03, 0x73, 0xdb});
        REQUIRE(shard::uuid::make_v3_uuid(shard::uuid::namespace_url, "foobar") == url_id);

        auto oid_id = shard::uuid(
            {0x7b, 0xf7, 0x94, 0xf0, 0xe7, 0x0a, 0x35, 0x0d, 0xaf, 0xaa, 0x7f, 0x84, 0x75, 0xad, 0xde, 0x77});
        REQUIRE(shard::uuid::make_v3_uuid(shard::uuid::namespace_oid, "foobar") == oid_id);

        auto x500_id = shard::uuid(
            {0xcc, 0x89, 0x85, 0xaa, 0x19, 0x64, 0x34, 0xea, 0xb3, 0xac, 0xf6, 0x80, 0x30, 0xe4, 0x4d, 0x79});
        REQUIRE(shard::uuid::make_v3_uuid(shard::uuid::namespace_x500, "foobar") == x500_id);
    }
}
