// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include "helpers/no_default.hpp"

#include <shard/optional.hpp>

#include <catch.hpp>

#include <memory>
#include <string>
#include <vector>

TEST_CASE("optional") {
    static_assert(sizeof(shard::optional<char>) == 2, "size check");
    static_assert(sizeof(shard::optional<int>) == 8, "size check");

    SECTION("storing class with no default constructor") {
        shard::optional<test::no_default> o;
        REQUIRE_FALSE(o.has_value());

        o.emplace(10, 20);
        REQUIRE(o.has_value());

        o.reset();
        REQUIRE_FALSE(o.has_value());
    }

    SECTION("emplacing values") {
        shard::optional<std::vector<int>> o;

        std::vector<int> expected1 {4, 4, 4};
        auto& values1 = o.emplace(3, 4);
        REQUIRE(values1 == expected1);

        std::vector<int> expected2 {5, 5};
        auto& values2 = o.emplace(2, 5);
        REQUIRE(values2 == expected2);
    }

    SECTION("optional string") {
        shard::optional<std::string> o;
        REQUIRE_FALSE(o.has_value());

        o = "foo";
        REQUIRE(o.has_value());
        REQUIRE(o.value() == "foo");
    }

    SECTION("constant value") {
        // default constructed
        {
            shard::optional<const int> o;
            REQUIRE_FALSE(o.has_value());

            o.emplace(4);
            REQUIRE(o.value() == 4);

            o.emplace(5);
            REQUIRE(o.value() == 5);

            o.reset();
            REQUIRE_FALSE(o.has_value());
        }

        // copy-constructed
        {
            const int x = 6;
            shard::optional<const int> o(x);
            REQUIRE(o.value() == 6);
        }

        // copy-constructed
        {
            const int x = 7;
            shard::optional<const int> o(std::move(x)); // NOLINT
            REQUIRE(o.value() == 7);
        }
        // no assignment allowed
    }

    SECTION("specifying default values") {
        shard::optional<int> o;

        REQUIRE_FALSE(o.has_value());
        REQUIRE(o.value_or(42) == 42);

        o = 4;
        REQUIRE(o.has_value());
        REQUIRE(o.value() == 4);
        REQUIRE(o.value_or(42) == 4);
    }

    SECTION("swapping values") {
        shard::optional<std::string> a;
        shard::optional<std::string> b;

        swap(a, b);
        REQUIRE_FALSE(a.has_value());
        REQUIRE_FALSE(b.has_value());

        a = "hello";
        REQUIRE(a.has_value());
        REQUIRE_FALSE(b.has_value());
        REQUIRE(a.value() == "hello");

        swap(a, b);
        REQUIRE_FALSE(a.has_value());
        REQUIRE(b.has_value());
        REQUIRE(b.value() == "hello");

        a = "bye";
        REQUIRE(a.has_value());
        REQUIRE(a.value() == "bye");

        swap(a, b);
        REQUIRE(a.has_value());
        REQUIRE(b.has_value());
        REQUIRE(a.value() == "hello");
        REQUIRE(b.value() == "bye");
    }

    SECTION("using make_optional with const lvalue") {
        const std::string s("foo");

        auto o = shard::make_optional(s);
        REQUIRE(o.has_value());
        REQUIRE(o.value() == "foo");

        *o = "bar";
        REQUIRE(s == "foo");
        REQUIRE(o.value() == "bar");
    }

    SECTION("using make_optional with lvalue") {
        std::string s("foo");

        auto o = shard::make_optional(s);
        REQUIRE(o.has_value());
        REQUIRE(o.value() == "foo");

        *o = "bar";
        REQUIRE(s == "foo");
    }

    SECTION("using make_optional with lvalue reference") {
        std::string s("foo");
        auto& ref = s;

        auto o = shard::make_optional(ref);
        REQUIRE(o.has_value());
        REQUIRE(o.value() == "foo");

        *o = "bar";
        REQUIRE(ref == "foo");
    }

    SECTION("using make_optional with rvalue") {
        auto ptr = std::make_unique<int>(10);

        auto o = shard::make_optional(std::move(ptr));
        REQUIRE(o.has_value());
        REQUIRE(ptr.get() == nullptr);
        REQUIRE(*o.value() == 10);
    }

    SECTION("operators") {
        // optional with value
        REQUIRE(shard::make_optional(1) == 1);
        REQUIRE(shard::make_optional(1) != 0);
        REQUIRE(1 == shard::make_optional(1));
        REQUIRE(0 != shard::make_optional(1));

        // 2 optionals
        REQUIRE(shard::make_optional(1) == shard::make_optional(1));
        REQUIRE(shard::make_optional(1) != shard::make_optional(0));
        REQUIRE(shard::make_optional(0) < shard::make_optional(1));
        REQUIRE(shard::make_optional(1) > shard::make_optional(0));
        REQUIRE(shard::make_optional(1) <= shard::make_optional(1));
        REQUIRE(shard::make_optional(1) >= shard::make_optional(1));

        // optional with nullopt
        REQUIRE(shard::optional<int> {} == shard::nullopt);
        REQUIRE(shard::nullopt == shard::optional<int> {});

        REQUIRE(shard::nullopt < shard::make_optional(1));
        REQUIRE_FALSE(shard::nullopt < shard::optional<int> {});
        REQUIRE_FALSE(shard::optional<int> {} < shard::nullopt);
        REQUIRE(shard::make_optional(1) > shard::nullopt);
        REQUIRE_FALSE(shard::nullopt > shard::optional<int> {});
        REQUIRE_FALSE(shard::optional<int> {} > shard::nullopt);

        REQUIRE(shard::nullopt <= shard::make_optional(1));
        REQUIRE(shard::nullopt <= shard::optional<int> {});
        REQUIRE(shard::optional<int> {} <= shard::nullopt);
        REQUIRE(shard::make_optional(1) >= shard::nullopt);
        REQUIRE(shard::nullopt >= shard::optional<int> {});
        REQUIRE(shard::optional<int> {} >= shard::nullopt);
    }

    SECTION("std::hash compatibility") {
        using hash_type = std::hash<shard::optional<int>>;
        auto h1 = hash_type {}(shard::make_optional(1));
        auto h2 = hash_type {}(shard::make_optional(1));
        REQUIRE(h1 == h2);

        auto h3 = hash_type {}(shard::optional<int> {});
        auto h4 = hash_type {}(shard::optional<int>(shard::nullopt));
        REQUIRE(h3 == h4);
    }
}
