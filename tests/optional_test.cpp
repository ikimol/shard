// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include "helpers/no_default.hpp"

#include <shard/optional.hpp>

#include <doctest.h>

#include <memory>
#include <string>
#include <vector>

TEST_SUITE("optional") {
    static_assert(sizeof(shard::optional<char>) == 2, "size check");
    static_assert(sizeof(shard::optional<int>) == 8, "size check");

    TEST_CASE("storing class with no default constructor") {
        shard::optional<test::no_default> o;
        CHECK_FALSE(o.has_value());

        o.emplace(10, 20);
        CHECK(o.has_value());

        o.reset();
        CHECK_FALSE(o.has_value());
    }

    TEST_CASE("emplacing values") {
        shard::optional<std::vector<int>> o;

        std::vector<int> expected1 {4, 4, 4};
        auto& values1 = o.emplace(3, 4);
        CHECK(values1 == expected1);

        std::vector<int> expected2 {5, 5};
        auto& values2 = o.emplace(2, 5);
        CHECK(values2 == expected2);
    }

    TEST_CASE("optional string") {
        shard::optional<std::string> o;
        CHECK_FALSE(o.has_value());

        o = "foo";
        CHECK(o.has_value());
        CHECK(o.value() == "foo");
    }

    TEST_CASE("constant value") {
        // default constructed
        {
            shard::optional<const int> o;
            CHECK_FALSE(o.has_value());

            o.emplace(4);
            CHECK(o.value() == 4);

            o.emplace(5);
            CHECK(o.value() == 5);

            o.reset();
            CHECK_FALSE(o.has_value());
        }

        // copy-constructed
        {
            const int x = 6;
            shard::optional<const int> o(x);
            CHECK(o.value() == 6);
        }

        // copy-constructed
        {
            const int x = 7;
            shard::optional<const int> o(std::move(x)); // NOLINT
            CHECK(o.value() == 7);
        }
        // no assignment allowed
    }

    TEST_CASE("specifying default values") {
        shard::optional<int> o;

        CHECK_FALSE(o.has_value());
        CHECK(o.value_or(42) == 42);

        o = 4;
        CHECK(o.has_value());
        CHECK(o.value() == 4);
        CHECK(o.value_or(42) == 4);
    }

    TEST_CASE("swapping values") {
        shard::optional<std::string> a;
        shard::optional<std::string> b;

        swap(a, b);
        CHECK_FALSE(a.has_value());
        CHECK_FALSE(b.has_value());

        a = "hello";
        CHECK(a.has_value());
        CHECK_FALSE(b.has_value());
        CHECK(a.value() == "hello");

        swap(a, b);
        CHECK_FALSE(a.has_value());
        CHECK(b.has_value());
        CHECK(b.value() == "hello");

        a = "bye";
        CHECK(a.has_value());
        CHECK(a.value() == "bye");

        swap(a, b);
        CHECK(a.has_value());
        CHECK(b.has_value());
        CHECK(a.value() == "hello");
        CHECK(b.value() == "bye");
    }

    TEST_CASE("using make_optional with const lvalue") {
        const std::string s("foo");

        auto o = shard::make_optional(s);
        CHECK(o.has_value());
        CHECK(o.value() == "foo");

        *o = "bar";
        CHECK(s == "foo");
        CHECK(o.value() == "bar");
    }

    TEST_CASE("using make_optional with lvalue") {
        std::string s("foo");

        auto o = shard::make_optional(s);
        CHECK(o.has_value());
        CHECK(o.value() == "foo");

        *o = "bar";
        CHECK(s == "foo");
    }

    TEST_CASE("using make_optional with lvalue reference") {
        std::string s("foo");
        auto& ref = s;

        auto o = shard::make_optional(ref);
        CHECK(o.has_value());
        CHECK(o.value() == "foo");

        *o = "bar";
        CHECK(ref == "foo");
    }

    TEST_CASE("using make_optional with rvalue") {
        auto ptr = std::make_unique<int>(10);

        auto o = shard::make_optional(std::move(ptr));
        CHECK(o.has_value());
        CHECK(ptr.get() == nullptr);
        CHECK(*o.value() == 10);
    }

    TEST_CASE("operators") {
        // optional with value
        CHECK(shard::make_optional(1) == 1);
        CHECK(shard::make_optional(1) != 0);
        CHECK(1 == shard::make_optional(1));
        CHECK(0 != shard::make_optional(1));

        // 2 optionals
        CHECK(shard::make_optional(1) == shard::make_optional(1));
        CHECK(shard::make_optional(1) != shard::make_optional(0));
        CHECK(shard::make_optional(0) < shard::make_optional(1));
        CHECK(shard::make_optional(1) > shard::make_optional(0));
        CHECK(shard::make_optional(1) <= shard::make_optional(1));
        CHECK(shard::make_optional(1) >= shard::make_optional(1));

        // optional with nullopt
        CHECK(shard::optional<int> {} == shard::nullopt);
        CHECK(shard::nullopt == shard::optional<int> {});

        CHECK(shard::nullopt < shard::make_optional(1));
        CHECK_FALSE(shard::nullopt < shard::optional<int> {});
        CHECK_FALSE(shard::optional<int> {} < shard::nullopt);
        CHECK(shard::make_optional(1) > shard::nullopt);
        CHECK_FALSE(shard::nullopt > shard::optional<int> {});
        CHECK_FALSE(shard::optional<int> {} > shard::nullopt);

        CHECK(shard::nullopt <= shard::make_optional(1));
        CHECK(shard::nullopt <= shard::optional<int> {});
        CHECK(shard::optional<int> {} <= shard::nullopt);
        CHECK(shard::make_optional(1) >= shard::nullopt);
        CHECK(shard::nullopt >= shard::optional<int> {});
        CHECK(shard::optional<int> {} >= shard::nullopt);
    }

    TEST_CASE("std::hash compatibility") {
        using hash_type = std::hash<shard::optional<int>>;
        auto h1 = hash_type {}(shard::make_optional(1));
        auto h2 = hash_type {}(shard::make_optional(1));
        CHECK(h1 == h2);

        auto h3 = hash_type {}(shard::optional<int> {});
        auto h4 = hash_type {}(shard::optional<int>(shard::nullopt));
        CHECK(h3 == h4);
    }
}
