// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/meta.hpp>

#include <doctest.h>

#include <cstddef>
#include <ostream>

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

TEST_SUITE("meta") {
    TEST_CASE("type_traits") {
        SUBCASE("is_integer") {
            CHECK(shard::is_integer<short>::value);
            CHECK(shard::is_integer<unsigned short>::value);
            CHECK(shard::is_integer<int>::value);
            CHECK(shard::is_integer<unsigned int>::value);
            CHECK(shard::is_integer<long>::value);
            CHECK(shard::is_integer<unsigned long>::value);
            CHECK(shard::is_integer<long long>::value);
            CHECK(shard::is_integer<unsigned long long>::value);

            CHECK_FALSE(shard::is_integer<bool>::value);
            CHECK_FALSE(shard::is_integer<float>::value);
            CHECK_FALSE(shard::is_integer<double>::value);
            CHECK_FALSE(shard::is_integer<void*>::value);
        }

        SUBCASE("is_bool") {
            CHECK(shard::is_bool<bool>::value);

            CHECK_FALSE(shard::is_bool<int>::value);
            CHECK_FALSE(shard::is_bool<void*>::value);
            CHECK_FALSE(shard::is_bool<float>::value);
        }

        SUBCASE("is_numeric") {
            CHECK(shard::is_numeric<int>::value);
            CHECK(shard::is_numeric<float>::value);
            CHECK(shard::is_numeric<double>::value);

            CHECK_FALSE(shard::is_numeric<bool>::value);
            CHECK_FALSE(shard::is_bool<void*>::value);
        }

        SUBCASE("is_streamable") {
            CHECK(shard::is_streamable<std::ostream, bool>::value);
            CHECK(shard::is_streamable<std::ostream, int>::value);
            CHECK(shard::is_streamable<std::ostream, float>::value);
            CHECK(shard::is_streamable<std::ostream, void*>::value);
            CHECK(shard::is_streamable<std::ostream, std::nullptr_t>::value);

            CHECK(shard::is_streamable<std::ostream, Widget>::value);

            CHECK_FALSE(shard::is_streamable<std::ostream, Gizmo>::value);
        }

        SUBCASE("are_same") {
            CHECK(shard::are_same<int, int, int>::value);
            CHECK_FALSE(shard::are_same<int, float, int>::value);
        }

        SUBCASE("not_type") {
            CHECK(shard::not_type<shard::is_bool<float>>::value);
            CHECK_FALSE(shard::not_type<shard::is_bool<bool>>::value);
        }

        SUBCASE("if_type") {
            CHECK(std::is_same<shard::if_type<shard::is_bool<bool>, char, void>, char>::value);
            CHECK(std::is_same<shard::if_type<shard::is_bool<float>, char, void>, void>::value);
        }

        SUBCASE("and_type") {
            CHECK(shard::and_type<shard::is_bool<bool>, shard::is_integer<long>>::value);
            CHECK_FALSE(shard::and_type<shard::is_bool<bool>, shard::is_numeric<void*>>::value);
            CHECK_FALSE(shard::and_type<shard::is_bool<int>, shard::is_numeric<void*>>::value);
        }

        SUBCASE("or_type") {
            CHECK(shard::or_type<shard::is_bool<bool>, shard::is_integer<long>>::value);
            CHECK(shard::or_type<shard::is_bool<bool>, shard::is_numeric<void*>>::value);
            CHECK(shard::or_type<shard::is_bool<int>, shard::is_bool<bool>>::value);
            CHECK_FALSE(shard::or_type<shard::is_bool<int>, shard::is_numeric<void*>>::value);
        }

        SUBCASE("result_of") {
            CHECK(std::is_same<shard::result_of_t<decltype(dummy_1)>, bool>::value);
            CHECK(std::is_same<shard::result_of_t<decltype(dummy_2)>, const char*>::value);
            CHECK(std::is_same<shard::result_of_t<decltype(&dummy_2)>, const char*>::value);
        }
    }

    TEST_CASE("typeid") {
        SUBCASE("default typespace") {
            CHECK(SHARD_TYPEID(int) == 0);
            CHECK(SHARD_TYPEID(unsigned int) == 1);
            CHECK(SHARD_TYPEID(float) == 2);
            CHECK(SHARD_TYPEID(double) == 3);
        }

        SUBCASE("separate typespaces") {
            CHECK(SHARD_TYPEID(void, int) == 0);
            CHECK(SHARD_TYPEID(void, unsigned int) == 1);

            CHECK(SHARD_TYPEID(char, int) == 0);
            CHECK(SHARD_TYPEID(char, unsigned int) == 1);
        }
    }
}
