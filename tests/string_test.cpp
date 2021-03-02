// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include "helpers/streamable.hpp"

#include <shard/string.hpp>

#include <doctest.h>

#include <set>
#include <vector>

TEST_SUITE("string") {
    TEST_CASE("case conversion") {
        CHECK(shard::to_lower_copy("FooBAR") == "foobar");
        CHECK(shard::to_upper_copy("FooBAR") == "FOOBAR");
        CHECK(shard::to_capitalized_copy("fooBaR") == "FooBaR");
        CHECK(shard::string::to_snake_case_copy("FooBarBaz") == "foo_bar_baz");
        CHECK(shard::string::to_snake_case_copy("fooBarBaz") == "foo_bar_baz");
    }

    TEST_CASE("comparisons") {
        SUBCASE("iequal") {
            CHECK(shard::ch::iequal('a', 'A'));
            CHECK(shard::ch::iequal('z', 'Z'));
            CHECK(shard::ch::iequal('1', '1'));
            CHECK_FALSE(shard::ch::iequal('a', 'B'));
        }

        SUBCASE("iequals") {
            CHECK(shard::iequals("foo", "FoO"));
            CHECK(shard::iequals("foo", "foo"));
            CHECK(shard::iequals("foo", "FOO"));
            CHECK_FALSE(shard::iequals("foo", "boo"));
        }

        SUBCASE("starts_with") {
            CHECK(shard::starts_with("foobar", "foo"));
            CHECK(shard::starts_with("foobar", "f"));
            CHECK(shard::starts_with("foobar", "foobar"));
        }

        SUBCASE("ends_with") {
            CHECK(shard::ends_with("foobar", "bar"));
            CHECK(shard::ends_with("foobar", "r"));
            CHECK(shard::ends_with("foobar", "foobar"));
        }

        SUBCASE("contains") {
            CHECK(shard::contains("foobar", "oo"));
            CHECK(shard::contains("foobar", "f"));
            CHECK(shard::contains("foobar", "r"));
            CHECK(shard::contains("foobar", "foobar"));
        }
    }

    TEST_CASE("fmt") {
        SUBCASE("buffered") {
            char buffer[64];

            CHECK(shard::sfmt(buffer, sizeof(buffer), "%d", 42) == "42");
            std::memset(buffer, '\0', 64);

            CHECK(shard::sfmt(buffer, sizeof(buffer), "%.2f", 3.1415) == "3.14");
            std::memset(buffer, '\0', 64);

            CHECK(shard::sfmt(buffer, sizeof(buffer), "%s", "foo") == "foo");
            std::memset(buffer, '\0', 64);
        }

        SUBCASE("dynamic") {
            CHECK(shard::fmt("%d", 42) == "42");
            CHECK(shard::fmt("%.3f", 3.1415) == "3.142");
            CHECK(shard::fmt("%s", "foo") == "foo");
        }

        SUBCASE("generator") {
            auto fmt = SHARD_MAKE_FMT(16, "x = %.2f");
            CHECK(fmt(3.1415f) == "x = 3.14");
            CHECK(fmt(1.4142f) == "x = 1.41");
        }
    }

    TEST_CASE("join") {
        SUBCASE("array") {
            bool a[3] = {true, false, true};
            CHECK(shard::join(a, " || ") == "true || false || true");
        }

        SUBCASE("vector") {
            std::vector<const char*> v = {"foo", "bar", "baz"};
            CHECK(shard::join(v, ", ") == "foo, bar, baz");
        }

        SUBCASE("set") {
            std::set<int> s = {1, 2, 3};
            CHECK(shard::join(s, " + ") == "1 + 2 + 3");
        }

        SUBCASE("user type") {
            std::vector<test::streamable> v;
            v.emplace_back("#1");
            v.emplace_back("#2");
            v.emplace_back("#3");
            CHECK(shard::join(v, ", ") == R"(streamable("#1"), streamable("#2"), streamable("#3"))");
        }
    }

    TEST_CASE("replace") {
        std::string s("foo bar baz");
        std::string expected("f00 bar baz");

        shard::replace_all(s, "o", "0");
        CHECK(s == expected);
        CHECK(shard::replace_all_copy(s, "a", "@") == "f00 b@r b@z");
    }

    TEST_CASE("split") {
        std::vector<std::string> v;
        shard::split("foo bar baz", std::back_inserter(v), ' ');
        CHECK(v.size() == 3);
        CHECK(v[0] == "foo");
        CHECK(v[1] == "bar");
        CHECK(v[2] == "baz");
    }

    TEST_CASE("to_string") {
        CHECK(shard::to_string(nullptr) == "(null)");
        CHECK(shard::to_string('a') == "a");
        CHECK(shard::to_string(false) == "false");
        CHECK(shard::to_string("foo") == "foo");
        CHECK(shard::to_string(std::string("bar")) == "bar");
        CHECK(shard::to_string(42) == "42");
        CHECK(shard::to_string(3.14f) == "3.140000");
        CHECK(shard::to_string(1.41) == "1.410000");
        CHECK(shard::to_string(test::streamable {"#0"}) == "streamable(\"#0\")");
    }

    TEST_CASE("trimming") {
        CHECK(shard::ltrim_copy("  foo") == "foo");
        CHECK(shard::ltrim_copy("\tfoo") == "foo");
        CHECK(shard::ltrim_copy("\nfoo") == "foo");
        CHECK(shard::ltrim_copy("\vfoo") == "foo");
        CHECK(shard::ltrim_copy("\ffoo") == "foo");
        CHECK(shard::ltrim_copy("\rfoo") == "foo");

        CHECK(shard::rtrim_copy("foo  ") == "foo");
        CHECK(shard::rtrim_copy("foo\t") == "foo");
        CHECK(shard::rtrim_copy("foo\n") == "foo");
        CHECK(shard::rtrim_copy("foo\v") == "foo");
        CHECK(shard::rtrim_copy("foo\f") == "foo");
        CHECK(shard::rtrim_copy("foo\r") == "foo");

        CHECK(shard::trim_copy("  foo  ") == "foo");
        CHECK(shard::trim_copy("\tfoo\t") == "foo");
        CHECK(shard::trim_copy("\nfoo\n") == "foo");
        CHECK(shard::trim_copy("\vfoo\v") == "foo");
        CHECK(shard::trim_copy("\ffoo\f") == "foo");
        CHECK(shard::trim_copy("\rfoo\r") == "foo");
    }
}