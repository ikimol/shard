// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/meta.hpp>

#include <doctest.h>

#include <array>
#include <deque>
#include <list>
#include <map>
#include <ostream>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>

struct Widget {
    const char* name = "?";
};

struct Gizmo {
    int id = 0;
};

inline std::ostream& operator<<(std::ostream& os, const Widget& w) {
    os << "Widget(" << w.name << ')';
    return os;
}

static bool dummy_1(int, char) {
    return false;
}

static const char* dummy_2(float) {
    return "dummy";
}

TEST_CASE("meta") {
    SUBCASE("type_traits") {
        SUBCASE("is_integer") {
            REQUIRE(shard::is_integer_v<short>);
            REQUIRE(shard::is_integer_v<unsigned short>);
            REQUIRE(shard::is_integer_v<int>);
            REQUIRE(shard::is_integer_v<unsigned int>);
            REQUIRE(shard::is_integer_v<long>);
            REQUIRE(shard::is_integer_v<unsigned long>);
            REQUIRE(shard::is_integer_v<long long>);
            REQUIRE(shard::is_integer_v<unsigned long long>);

            REQUIRE_FALSE(shard::is_integer_v<bool>);
            REQUIRE_FALSE(shard::is_integer_v<float>);
            REQUIRE_FALSE(shard::is_integer_v<double>);
            REQUIRE_FALSE(shard::is_integer_v<void*>);
        }

        SUBCASE("is_bool") {
            REQUIRE(shard::is_bool_v<bool>);

            REQUIRE_FALSE(shard::is_bool_v<int>);
            REQUIRE_FALSE(shard::is_bool_v<void*>);
            REQUIRE_FALSE(shard::is_bool_v<float>);
        }

        SUBCASE("is_numeric") {
            REQUIRE(shard::is_numeric_v<int>);
            REQUIRE(shard::is_numeric_v<float>);
            REQUIRE(shard::is_numeric_v<double>);

            REQUIRE_FALSE(shard::is_numeric_v<bool>);
            REQUIRE_FALSE(shard::is_numeric_v<void*>);
        }

        SUBCASE("is_streamable") {
            REQUIRE(shard::is_streamable_v<std::ostream, bool>);
            REQUIRE(shard::is_streamable_v<std::ostream, int>);
            REQUIRE(shard::is_streamable_v<std::ostream, float>);
            REQUIRE(shard::is_streamable_v<std::ostream, void*>);
            REQUIRE(shard::is_streamable_v<std::ostream, std::nullptr_t>);

            REQUIRE(shard::is_streamable_v<std::ostream, Widget>);

            REQUIRE_FALSE(shard::is_streamable_v<std::ostream, Gizmo>);
        }

        SUBCASE("is_specialization_of") {
            REQUIRE(shard::is_specialization_of_v<std::optional<int>, std::optional>);
            REQUIRE(shard::is_specialization_of_v<std::vector<int>, std::vector>);
            REQUIRE(shard::is_specialization_of_v<std::unordered_map<int, bool>, std::unordered_map>);

            REQUIRE_FALSE(shard::is_specialization_of_v<std::vector<int>, std::queue>);
        }

        SUBCASE("is_optional") {
            REQUIRE(shard::is_optional_v<std::optional<int>>);
            REQUIRE(shard::is_optional_v<std::optional<float>>);

            REQUIRE_FALSE(shard::is_optional_v<int>);
            REQUIRE_FALSE(shard::is_optional_v<std::nullopt_t>);
        }

        SUBCASE("has_begin_end") {
            REQUIRE(shard::has_begin_end_v<std::array<int, 3>>);
            REQUIRE(shard::has_begin_end_v<std::vector<int>>);
            REQUIRE(shard::has_begin_end_v<std::list<int>>);
            REQUIRE(shard::has_begin_end_v<std::deque<int>>);
            REQUIRE(shard::has_begin_end_v<std::set<int>>);
            REQUIRE(shard::has_begin_end_v<std::unordered_map<int, int>>);

            REQUIRE_FALSE(shard::has_begin_end_v<std::queue<int>>);
            REQUIRE_FALSE(shard::has_begin_end_v<std::stack<int>>);
        }

        SUBCASE("has_key_value_pair") {
            REQUIRE(shard::has_key_value_pair_v<std::map<int, int>>);
            REQUIRE(shard::has_key_value_pair_v<std::unordered_map<int, int>>);

            REQUIRE_FALSE(shard::has_key_value_pair_v<std::vector<int>>);
            REQUIRE_FALSE(shard::has_key_value_pair_v<std::set<int>>);
        }

        SUBCASE("are_same") {
            REQUIRE(shard::are_same_v<int, int, int>);
            REQUIRE_FALSE(shard::are_same_v<int, float, int>);
        }

        SUBCASE("unqualified") {
            REQUIRE(std::is_same_v<shard::unqualified_t<const volatile int&>, int>);
            REQUIRE(std::is_same_v<shard::unqualified_t<int&&>, int>);
            REQUIRE(std::is_same_v<shard::unqualified_t<int[]>, int[]>);
        }

        SUBCASE("not_type") {
            REQUIRE(shard::not_v<shard::is_bool<float>>);
            REQUIRE_FALSE(shard::not_v<shard::is_bool<bool>>);
        }

        SUBCASE("if_type") {
            REQUIRE(std::is_same_v<shard::if_t<shard::is_bool<bool>, char, void>, char>);
            REQUIRE(std::is_same_v<shard::if_t<shard::is_bool<float>, char, void>, void>);
        }

        SUBCASE("and_type") {
            REQUIRE(shard::and_v<shard::is_bool<bool>, shard::is_integer<long>>);
            REQUIRE_FALSE(shard::and_v<shard::is_bool<bool>, shard::is_numeric<void*>>);
            REQUIRE_FALSE(shard::and_v<shard::is_bool<int>, shard::is_numeric<void*>>);
        }

        SUBCASE("or_type") {
            REQUIRE(shard::or_v<shard::is_bool<bool>, shard::is_integer<long>>);
            REQUIRE(shard::or_v<shard::is_bool<bool>, shard::is_numeric<void*>>);
            REQUIRE(shard::or_v<shard::is_bool<int>, shard::is_bool<bool>>);
            REQUIRE_FALSE(shard::or_v<shard::is_bool<int>, shard::is_numeric<void*>>);
        }

        SUBCASE("result_of") {
            REQUIRE(std::is_same_v<shard::result_of_t<decltype(dummy_1)>, bool>);
            REQUIRE(std::is_same_v<shard::result_of_t<decltype(dummy_2)>, const char*>);
            REQUIRE(std::is_same_v<shard::result_of_t<decltype(&dummy_2)>, const char*>);
        }
    }

    SUBCASE("type_id") {
        SUBCASE("conversion") {
            auto id = shard::meta::typespace<void>::id<int>();
            std::size_t value = id;
            REQUIRE(value == 0);
        }

        SUBCASE("default typespace") {
            REQUIRE(SHARD_TYPEID(int).value() == 0);
            REQUIRE(SHARD_TYPEID(unsigned int).value() == 1);
            REQUIRE(SHARD_TYPEID(float).value() == 2);
            REQUIRE(SHARD_TYPEID(double).value() == 3);
        }

        SUBCASE("separate typespaces") {
            REQUIRE(shard::meta::typespace<void>::id<int>() == 0);
            REQUIRE(shard::meta::typespace<void>::id<unsigned int>() == 1);

            REQUIRE(shard::meta::typespace<char>::id<int>() == 0);
            REQUIRE(shard::meta::typespace<char>::id<unsigned int>() == 1);
        }

        SUBCASE("relational operators") {
            REQUIRE(SHARD_TYPEID(int) == SHARD_TYPEID(int));
            REQUIRE(SHARD_TYPEID(int) != SHARD_TYPEID(short));
            REQUIRE(SHARD_TYPEID(int) < SHARD_TYPEID(short));
        }

        SUBCASE("hash") {
            auto hash = std::hash<decltype(SHARD_TYPEID(int))> {}(SHARD_TYPEID(int));
            REQUIRE(hash == SHARD_TYPEID(int));
        }
    }
}
