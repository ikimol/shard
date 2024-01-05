// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/utility/defer.hpp>
#include <shard/utility/exception_guard.hpp>
#include <shard/utility/hash_value.hpp>
#include <shard/utility/lazy.hpp>

#include <stdexcept>
#include <string>

#include <doctest.h>

TEST_CASE("utility") {
    SUBCASE("defer") {
        bool called = false;
        {
            auto _ = shard::defer([&] { called = true; });
        }
        REQUIRE(called);
    }

    SUBCASE("exception_guard") {
        bool did_rollback = false;

        SUBCASE("throw") {
            {
                auto guard = shard::make_exception_guard([&] { did_rollback = true; });
                try {
                    throw std::runtime_error("test error");
                } catch (std::exception& e) {}
            }
            REQUIRE(did_rollback);
        }

        SUBCASE("nothrow") {
            {
                auto guard = shard::make_exception_guard([&] { did_rollback = true; });
                try {
                    guard.dismiss();
                } catch (std::exception& e) {}
            }
            REQUIRE_FALSE(did_rollback);
        }
    }

    SUBCASE("hashing") {
        auto i1 = shard::hash_value(1, 2, 3);
        auto i2 = shard::hash_value(1, 2, 3);
        REQUIRE(i1 == i2);

        auto str1 = shard::hash_value(std::string("foo"));
        auto str2 = shard::hash_value(std::string("FOO"));
        REQUIRE(str1 != str2);
    }

    SUBCASE("lazy") {
        SUBCASE("called once") {
            constexpr int r_value = 42;

            int call_count = 0;

            shard::lazy<int> value([&] {
                ++call_count;
                return r_value;
            });

            REQUIRE(*value == r_value);
            REQUIRE(*value == r_value);
            REQUIRE(*value == r_value);
            REQUIRE(call_count == 1);
        }

        SUBCASE("value initialized") {
            shard::lazy<int> value = 3;
            REQUIRE(*value == 3);
        }
    }
}