// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/meta.hpp>

#include <doctest.h>

#include <array>
#include <cstddef>
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
            REQUIRE(shard::is_integer<short>::value);
            REQUIRE(shard::is_integer<unsigned short>::value);
            REQUIRE(shard::is_integer<int>::value);
            REQUIRE(shard::is_integer<unsigned int>::value);
            REQUIRE(shard::is_integer<long>::value);
            REQUIRE(shard::is_integer<unsigned long>::value);
            REQUIRE(shard::is_integer<long long>::value);
            REQUIRE(shard::is_integer<unsigned long long>::value);

            REQUIRE_FALSE(shard::is_integer<bool>::value);
            REQUIRE_FALSE(shard::is_integer<float>::value);
            REQUIRE_FALSE(shard::is_integer<double>::value);
            REQUIRE_FALSE(shard::is_integer<void*>::value);
        }

        SUBCASE("is_bool") {
            REQUIRE(shard::is_bool<bool>::value);

            REQUIRE_FALSE(shard::is_bool<int>::value);
            REQUIRE_FALSE(shard::is_bool<void*>::value);
            REQUIRE_FALSE(shard::is_bool<float>::value);
        }

        SUBCASE("is_numeric") {
            REQUIRE(shard::is_numeric<int>::value);
            REQUIRE(shard::is_numeric<float>::value);
            REQUIRE(shard::is_numeric<double>::value);

            REQUIRE_FALSE(shard::is_numeric<bool>::value);
            REQUIRE_FALSE(shard::is_bool<void*>::value);
        }

        SUBCASE("is_streamable") {
            REQUIRE(shard::is_streamable<std::ostream, bool>::value);
            REQUIRE(shard::is_streamable<std::ostream, int>::value);
            REQUIRE(shard::is_streamable<std::ostream, float>::value);
            REQUIRE(shard::is_streamable<std::ostream, void*>::value);
            REQUIRE(shard::is_streamable<std::ostream, std::nullptr_t>::value);

            REQUIRE(shard::is_streamable<std::ostream, Widget>::value);

            REQUIRE_FALSE(shard::is_streamable<std::ostream, Gizmo>::value);
        }

        SUBCASE("has_begin_end") {
            REQUIRE(shard::has_begin_end<std::array<int, 3>>::value);
            REQUIRE(shard::has_begin_end<std::vector<int>>::value);
            REQUIRE(shard::has_begin_end<std::list<int>>::value);
            REQUIRE(shard::has_begin_end<std::deque<int>>::value);
            REQUIRE(shard::has_begin_end<std::set<int>>::value);
            REQUIRE(shard::has_begin_end<std::unordered_map<int, int>>::value);

            REQUIRE_FALSE(shard::has_begin_end<std::queue<int>>::value);
            REQUIRE_FALSE(shard::has_begin_end<std::stack<int>>::value);
        }

        SUBCASE("has_key_value_pair") {
            REQUIRE(shard::has_key_value_pair<std::map<int, int>>::value);
            REQUIRE(shard::has_key_value_pair<std::unordered_map<int, int>>::value);

            REQUIRE_FALSE(shard::has_key_value_pair<std::vector<int>>::value);
            REQUIRE_FALSE(shard::has_key_value_pair<std::set<int>>::value);
        }

        SUBCASE("are_same") {
            REQUIRE(shard::are_same<int, int, int>::value);
            REQUIRE_FALSE(shard::are_same<int, float, int>::value);
        }

        SUBCASE("not_type") {
            REQUIRE(shard::not_type<shard::is_bool<float>>::value);
            REQUIRE_FALSE(shard::not_type<shard::is_bool<bool>>::value);
        }

        SUBCASE("if_type") {
            REQUIRE(std::is_same<shard::if_type<shard::is_bool<bool>, char, void>, char>::value);
            REQUIRE(std::is_same<shard::if_type<shard::is_bool<float>, char, void>, void>::value);
        }

        SUBCASE("and_type") {
            REQUIRE(shard::and_type<shard::is_bool<bool>, shard::is_integer<long>>::value);
            REQUIRE_FALSE(shard::and_type<shard::is_bool<bool>, shard::is_numeric<void*>>::value);
            REQUIRE_FALSE(shard::and_type<shard::is_bool<int>, shard::is_numeric<void*>>::value);
        }

        SUBCASE("or_type") {
            REQUIRE(shard::or_type<shard::is_bool<bool>, shard::is_integer<long>>::value);
            REQUIRE(shard::or_type<shard::is_bool<bool>, shard::is_numeric<void*>>::value);
            REQUIRE(shard::or_type<shard::is_bool<int>, shard::is_bool<bool>>::value);
            REQUIRE_FALSE(shard::or_type<shard::is_bool<int>, shard::is_numeric<void*>>::value);
        }

        SUBCASE("result_of") {
            REQUIRE(std::is_same<shard::result_of_t<decltype(dummy_1)>, bool>::value);
            REQUIRE(std::is_same<shard::result_of_t<decltype(dummy_2)>, const char*>::value);
            REQUIRE(std::is_same<shard::result_of_t<decltype(&dummy_2)>, const char*>::value);
        }
    }

    SUBCASE("typeid") {
        SUBCASE("default typespace") {
            REQUIRE(SHARD_TYPEID(int) == 0);
            REQUIRE(SHARD_TYPEID(unsigned int) == 1);
            REQUIRE(SHARD_TYPEID(float) == 2);
            REQUIRE(SHARD_TYPEID(double) == 3);
        }

        SUBCASE("separate typespaces") {
            REQUIRE(SHARD_TYPEID(void, int) == 0);
            REQUIRE(SHARD_TYPEID(void, unsigned int) == 1);

            REQUIRE(SHARD_TYPEID(char, int) == 0);
            REQUIRE(SHARD_TYPEID(char, unsigned int) == 1);
        }
    }
}
