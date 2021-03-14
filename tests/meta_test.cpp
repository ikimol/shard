// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/meta.hpp>

#include <catch.hpp>

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

TEST_CASE("meta") {
    SECTION("type_traits") {
        SECTION("is_integer") {
            REQUIRE(shard::is_integer<short>::value);
            REQUIRE(shard::is_integer<unsigned short>::value);
            REQUIRE(shard::is_integer<int>::value);
            REQUIRE(shard::is_integer<unsigned int>::value);
            REQUIRE(shard::is_integer<long>::value);
            REQUIRE(shard::is_integer<unsigned long>::value);
            REQUIRE(shard::is_integer<long long>::value);
            REQUIRE(shard::is_integer<unsigned long long>::value);

            CHECK_FALSE(shard::is_integer<bool>::value);
            CHECK_FALSE(shard::is_integer<float>::value);
            CHECK_FALSE(shard::is_integer<double>::value);
            CHECK_FALSE(shard::is_integer<void*>::value);
        }

        SECTION("is_bool") {
            REQUIRE(shard::is_bool<bool>::value);

            CHECK_FALSE(shard::is_bool<int>::value);
            CHECK_FALSE(shard::is_bool<void*>::value);
            CHECK_FALSE(shard::is_bool<float>::value);
        }

        SECTION("is_numeric") {
            REQUIRE(shard::is_numeric<int>::value);
            REQUIRE(shard::is_numeric<float>::value);
            REQUIRE(shard::is_numeric<double>::value);

            CHECK_FALSE(shard::is_numeric<bool>::value);
            CHECK_FALSE(shard::is_bool<void*>::value);
        }

        SECTION("is_streamable") {
            REQUIRE(shard::is_streamable<std::ostream, bool>::value);
            REQUIRE(shard::is_streamable<std::ostream, int>::value);
            REQUIRE(shard::is_streamable<std::ostream, float>::value);
            REQUIRE(shard::is_streamable<std::ostream, void*>::value);
            REQUIRE(shard::is_streamable<std::ostream, std::nullptr_t>::value);

            REQUIRE(shard::is_streamable<std::ostream, Widget>::value);

            CHECK_FALSE(shard::is_streamable<std::ostream, Gizmo>::value);
        }

        SECTION("are_same") {
            REQUIRE(shard::are_same<int, int, int>::value);
            CHECK_FALSE(shard::are_same<int, float, int>::value);
        }

        SECTION("not_type") {
            REQUIRE(shard::not_type<shard::is_bool<float>>::value);
            CHECK_FALSE(shard::not_type<shard::is_bool<bool>>::value);
        }

        SECTION("if_type") {
            REQUIRE(std::is_same<shard::if_type<shard::is_bool<bool>, char, void>, char>::value);
            REQUIRE(std::is_same<shard::if_type<shard::is_bool<float>, char, void>, void>::value);
        }

        SECTION("and_type") {
            REQUIRE(shard::and_type<shard::is_bool<bool>, shard::is_integer<long>>::value);
            CHECK_FALSE(shard::and_type<shard::is_bool<bool>, shard::is_numeric<void*>>::value);
            CHECK_FALSE(shard::and_type<shard::is_bool<int>, shard::is_numeric<void*>>::value);
        }

        SECTION("or_type") {
            REQUIRE(shard::or_type<shard::is_bool<bool>, shard::is_integer<long>>::value);
            REQUIRE(shard::or_type<shard::is_bool<bool>, shard::is_numeric<void*>>::value);
            REQUIRE(shard::or_type<shard::is_bool<int>, shard::is_bool<bool>>::value);
            CHECK_FALSE(shard::or_type<shard::is_bool<int>, shard::is_numeric<void*>>::value);
        }

        SECTION("result_of") {
            REQUIRE(std::is_same<shard::result_of_t<decltype(dummy_1)>, bool>::value);
            REQUIRE(std::is_same<shard::result_of_t<decltype(dummy_2)>, const char*>::value);
            REQUIRE(std::is_same<shard::result_of_t<decltype(&dummy_2)>, const char*>::value);
        }
    }

    SECTION("typeid") {
        SECTION("default typespace") {
            REQUIRE(SHARD_TYPEID(int) == 0);
            REQUIRE(SHARD_TYPEID(unsigned int) == 1);
            REQUIRE(SHARD_TYPEID(float) == 2);
            REQUIRE(SHARD_TYPEID(double) == 3);
        }

        SECTION("separate typespaces") {
            REQUIRE(SHARD_TYPEID(void, int) == 0);
            REQUIRE(SHARD_TYPEID(void, unsigned int) == 1);

            REQUIRE(SHARD_TYPEID(char, int) == 0);
            REQUIRE(SHARD_TYPEID(char, unsigned int) == 1);
        }
    }
}
