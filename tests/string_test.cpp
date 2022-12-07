// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include "helpers/streamable.hpp"

#include <shard/string.hpp>

#include <doctest.h>

#include <cstring>
#include <set>
#include <vector>

static std::string format_helper(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    auto message = shard::fmt(format, ap);
    va_end(ap);
    return message;
}

TEST_CASE("string") {
    SUBCASE("case conversion") {
        REQUIRE(shard::to_lower_copy("FooBAR") == "foobar");
        REQUIRE(shard::to_upper_copy("FooBAR") == "FOOBAR");
        REQUIRE(shard::to_capitalized_copy("fooBaR") == "FooBaR");
        REQUIRE(shard::string::to_snake_case_copy("FooBarBaz") == "foo_bar_baz");
        REQUIRE(shard::string::to_snake_case_copy("fooBarBaz") == "foo_bar_baz");
    }

    SUBCASE("comparisons") {
        SUBCASE("iequal") {
            REQUIRE(shard::ch::iequal('a', 'A'));
            REQUIRE(shard::ch::iequal('z', 'Z'));
            REQUIRE(shard::ch::iequal('1', '1'));
            REQUIRE_FALSE(shard::ch::iequal('a', 'B'));
        }

        SUBCASE("iequals") {
            REQUIRE(shard::iequals("foo", "FoO"));
            REQUIRE(shard::iequals("foo", "foo"));
            REQUIRE(shard::iequals("foo", "FOO"));
            REQUIRE_FALSE(shard::iequals("foo", "boo"));
        }

        SUBCASE("starts_with") {
            REQUIRE(shard::starts_with("foobar", "foo"));
            REQUIRE(shard::starts_with("foobar", "f"));
            REQUIRE(shard::starts_with("foobar", "foobar"));
        }

        SUBCASE("ends_with") {
            REQUIRE(shard::ends_with("foobar", "bar"));
            REQUIRE(shard::ends_with("foobar", "r"));
            REQUIRE(shard::ends_with("foobar", "foobar"));
        }

        SUBCASE("contains") {
            REQUIRE(shard::contains("foobar", "oo"));
            REQUIRE(shard::contains("foobar", "f"));
            REQUIRE(shard::contains("foobar", "r"));
            REQUIRE(shard::contains("foobar", "foobar"));
        }
    }

    SUBCASE("fmt") {
        SUBCASE("buffered") {
            char buffer[64];

            REQUIRE(shard::sfmt(buffer, sizeof(buffer), "%d", 42) == "42");
            std::memset(buffer, '\0', 64);

            REQUIRE(shard::sfmt(buffer, sizeof(buffer), "%.2f", 3.1415) == "3.14");
            std::memset(buffer, '\0', 64);

            REQUIRE(shard::sfmt(buffer, sizeof(buffer), "%s", "foo") == "foo");
            std::memset(buffer, '\0', 64);
        }

        SUBCASE("dynamic") {
            REQUIRE(shard::fmt("%d", 42) == "42");
            REQUIRE(shard::fmt("%.3f", 3.1415) == "3.142");
            REQUIRE(shard::fmt("%s", "foo") == "foo");
        }

        SUBCASE("va_list") {
            REQUIRE(format_helper("%d", 42) == "42");
            REQUIRE(format_helper("%.3f", 3.1415) == "3.142");
            REQUIRE(format_helper("%s", "foo") == "foo");
        }

        SUBCASE("generator") {
            SUBCASE("same length") {
                auto fmt = SHARD_MAKE_FMT(16, "x = %.2f");
                REQUIRE(fmt(3.1415f) == "x = 3.14");
                REQUIRE(fmt(1.4142f) == "x = 1.41");
            }

            SUBCASE("variable length") {
                auto fmt = SHARD_MAKE_FMT(16, "s = %s");
                REQUIRE(fmt("foobar") == "s = foobar");
                REQUIRE(fmt("baz") == "s = baz");
            }
        }
    }

    SUBCASE("join") {
        SUBCASE("array") {
            bool a[3] = {true, false, true};
            REQUIRE(shard::join(a, " || ") == "true || false || true");
        }

        SUBCASE("vector") {
            std::vector<const char*> v = {"foo", "bar", "baz"};
            REQUIRE(shard::join(v, ", ") == "foo, bar, baz");
        }

        SUBCASE("set") {
            std::set<int> s = {1, 2, 3};
            REQUIRE(shard::join(s, " + ") == "1 + 2 + 3");
        }

        SUBCASE("user type") {
            std::vector<test::streamable> v;
            v.emplace_back("#1");
            v.emplace_back("#2");
            v.emplace_back("#3");
            REQUIRE(shard::join(v, ", ") == R"(streamable("#1"), streamable("#2"), streamable("#3"))");
        }
    }

    SUBCASE("replace") {
        std::string s("foo bar baz");
        std::string expected("f00 bar baz");

        shard::replace_all(s, "o", "0");
        REQUIRE(s == expected);
        REQUIRE(shard::replace_all_copy(s, "a", "@") == "f00 b@r b@z");
    }

    SUBCASE("split") {
        std::vector<std::string> v;
        shard::split("foo bar baz", std::back_inserter(v), ' ');
        REQUIRE(v.size() == 3);
        REQUIRE(v[0] == "foo");
        REQUIRE(v[1] == "bar");
        REQUIRE(v[2] == "baz");
    }

    SUBCASE("to_number") {
        REQUIRE(shard::to_int("42") == 42);
        REQUIRE(shard::to_int("-42") == -42);
        REQUIRE_FALSE(shard::to_int("42.2").has_value());
        REQUIRE_FALSE(shard::to_int("foo").has_value());

        REQUIRE(shard::to_unsigned("42") == 42);
        REQUIRE_FALSE(shard::to_unsigned("-42").has_value());
        REQUIRE_FALSE(shard::to_unsigned("42.2").has_value());
        REQUIRE_FALSE(shard::to_unsigned("foo").has_value());

        REQUIRE(shard::to_float("42") == 42);
        REQUIRE(shard::to_float("-42") == -42);
        REQUIRE(shard::to_float("42.2") == 42.2f);
        REQUIRE_FALSE(shard::to_float("foo").has_value());

        REQUIRE(shard::to_double("42") == 42);
        REQUIRE(shard::to_double("-42") == -42);
        REQUIRE(shard::to_double("42.2") == 42.2);
        REQUIRE_FALSE(shard::to_double("foo").has_value());
    }

    SUBCASE("to_string") {
        REQUIRE(shard::to_string(nullptr) == "(null)");
        REQUIRE(shard::to_string('a') == "a");
        REQUIRE(shard::to_string(false) == "false");
        REQUIRE(shard::to_string("foo") == "foo");
        REQUIRE(shard::to_string(std::string("bar")) == "bar");
        REQUIRE(shard::to_string(42) == "42");
        REQUIRE(shard::to_string(3.14f) == "3.140000");
        REQUIRE(shard::to_string(1.41) == "1.410000");
        REQUIRE(shard::to_string(test::streamable {"#0"}) == "streamable(\"#0\")");
    }

    SUBCASE("trimming") {
        REQUIRE(shard::ltrim_copy("  foo") == "foo");
        REQUIRE(shard::ltrim_copy("\tfoo") == "foo");
        REQUIRE(shard::ltrim_copy("\nfoo") == "foo");
        REQUIRE(shard::ltrim_copy("\vfoo") == "foo");
        REQUIRE(shard::ltrim_copy("\ffoo") == "foo");
        REQUIRE(shard::ltrim_copy("\rfoo") == "foo");

        REQUIRE(shard::rtrim_copy("foo  ") == "foo");
        REQUIRE(shard::rtrim_copy("foo\t") == "foo");
        REQUIRE(shard::rtrim_copy("foo\n") == "foo");
        REQUIRE(shard::rtrim_copy("foo\v") == "foo");
        REQUIRE(shard::rtrim_copy("foo\f") == "foo");
        REQUIRE(shard::rtrim_copy("foo\r") == "foo");

        REQUIRE(shard::trim_copy("  foo  ") == "foo");
        REQUIRE(shard::trim_copy("\tfoo\t") == "foo");
        REQUIRE(shard::trim_copy("\nfoo\n") == "foo");
        REQUIRE(shard::trim_copy("\vfoo\v") == "foo");
        REQUIRE(shard::trim_copy("\ffoo\f") == "foo");
        REQUIRE(shard::trim_copy("\rfoo\r") == "foo");
    }
}