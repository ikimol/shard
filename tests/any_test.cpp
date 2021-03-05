// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include "helpers/addresser.hpp"
#include "helpers/counter.hpp"

#include <shard/any.hpp>

#include <doctest.h>

#include <string>

TEST_SUITE("any") {
    TEST_CASE("default constructor") {
        shard::any a;

        CHECK_FALSE(a.has_value());
        CHECK(shard::any_cast<int>(&a) == nullptr);
        CHECK(a.type() == typeid(void));
    }

    TEST_CASE("copy constructor") {
        std::string s = "foobar";
        shard::any original = s;
        shard::any copy = original;

        CHECK(copy.has_value());
        CHECK(original.type() == copy.type());
        CHECK(shard::any_cast<std::string>(copy) == shard::any_cast<std::string>(original));
        CHECK(shard::any_cast<std::string>(copy) == s);
        CHECK(shard::any_cast<std::string>(&original) != shard::any_cast<std::string>(&copy));
    }

    TEST_CASE("move constructor") {
        shard::any original(test::counter {});
        test::counter::reset();

        shard::any moved_to(std::move(original));

        CHECK_FALSE(original.has_value()); /* NOLINT */
        CHECK(moved_to.has_value());
        CHECK(moved_to.type() == typeid(test::counter));
        CHECK(shard::any_cast<test::counter>(&original) == nullptr);
        CHECK(shard::any_cast<test::counter>(&moved_to) != nullptr);
        CHECK(test::counter::copy_constructor == 0);
        CHECK(test::counter::move_constructor == 0);

        test::counter::reset();
    }

    TEST_CASE("forwarding constructor") {
        std::string s = "foobar";
        shard::any a = s;

        CHECK(a.has_value());
        CHECK(a.type() == typeid(std::string));
        CHECK(shard::any_cast<int>(&a) == nullptr);
        CHECK(shard::any_cast<std::string>(&a) != nullptr);
        CHECK(shard::any_cast<std::string>(a) == s);
        CHECK(shard::any_cast<std::string>(&a) != &s);
    }

    TEST_CASE("copy assignment") {
        std::string s = "foobar";
        shard::any original = s;
        shard::any copy;
        shard::any* result = &(copy = original);

        CHECK(copy.has_value());
        CHECK(original.type() == copy.type());
        CHECK(shard::any_cast<std::string>(copy) == shard::any_cast<std::string>(original));
        CHECK(shard::any_cast<std::string>(copy) == s);
        CHECK(shard::any_cast<std::string>(&original) != shard::any_cast<std::string>(&copy));
        CHECK(&copy == result);
    }

    TEST_CASE("move assignment") {
        shard::any original = test::counter();
        shard::any moved_to;
        test::counter::reset();
        moved_to = std::move(original);

        CHECK_FALSE(original.has_value()); /* NOLINT */
        CHECK(moved_to.has_value());
        CHECK(moved_to.type() == typeid(test::counter));
        CHECK(shard::any_cast<test::counter>(&moved_to) != nullptr);
        CHECK(test::counter::copy_constructor == 0);
        CHECK(test::counter::move_constructor == 0);

        test::counter::reset();
    }

    TEST_CASE("forwarding assignment") {
        std::string s = "foobar";
        shard::any a;
        shard::any* result = &(a = s);

        CHECK(a.has_value());
        CHECK(a.type() == typeid(std::string));
        CHECK(shard::any_cast<int>(&a) == nullptr);
        CHECK(shard::any_cast<std::string>(&a) != nullptr);
        CHECK(shard::any_cast<std::string>(a) == s);
        CHECK(shard::any_cast<std::string>(&a) != &s);
        CHECK(result == &a);
    }

    TEST_CASE("emplace") {
        shard::any a;
        auto& s = a.emplace<std::string>("foobar");

        CHECK(a.has_value());
        CHECK(a.type() == typeid(std::string));
        CHECK(shard::any_cast<int>(&a) == nullptr);
        CHECK(shard::any_cast<std::string>(&a) != nullptr);
        CHECK(shard::any_cast<std::string>(a) == s);
        CHECK(shard::any_cast<std::string>(&a) == &s);

        // assign temporary

        test::counter::reset();
        {
            shard::any tmp = test::counter();
            CHECK(test::counter::default_constructor == 1);
            CHECK(test::counter::copy_constructor == 0);
            CHECK(test::counter::move_constructor == 1);
        }
        CHECK(test::counter::destructor == 2);

        // emplace type

        test::counter::reset();
        {
            shard::any tmp;
            tmp.emplace<test::counter>();
            CHECK(test::counter::default_constructor == 1);
            CHECK(test::counter::copy_constructor == 0);
            CHECK(test::counter::move_constructor == 0);
        }
        CHECK(test::counter::destructor == 1);
        test::counter::reset();
    }

    TEST_CASE("swap") {
        std::string s = "foobar";
        shard::any original = s;
        shard::any swapped;
        auto original_ptr = shard::any_cast<std::string>(&original);
        original.swap(swapped);
        shard::any* result = &original;

        CHECK_FALSE(original.has_value());
        CHECK(swapped.has_value());
        CHECK(swapped.type() == typeid(std::string));
        CHECK(shard::any_cast<std::string>(swapped) == s);
        CHECK(original_ptr != nullptr);
        CHECK(original_ptr == shard::any_cast<std::string>(&swapped));
        CHECK(result == &original);

        test::counter::reset();
        {
            shard::any copy1 = test::counter();
            shard::any copy2 = test::counter();
            auto count = test::counter::copy_constructor;
            swap(copy1, copy2);
            CHECK(count == test::counter::copy_constructor);
        }
        test::counter::reset();
    }

    TEST_CASE("cast to reference types") {
        shard::any a = 0;
        const shard::any b = a;

        int& a_ref = shard::any_cast<int&>(a);
        const int& a_cref = shard::any_cast<const int&>(a);

        CHECK(&a_ref == &a_cref);

        const int& b_cref = shard::any_cast<const int&>(b);

        CHECK(&a_ref != &b_cref);

        ++a_ref;
        int incremented = shard::any_cast<int>(a);

        CHECK(incremented == 1);

        CHECK_THROWS_AS(shard::any_cast<char&>(a), shard::bad_any_cast);
        CHECK_THROWS_AS(shard::any_cast<const char&>(b), shard::bad_any_cast);
    }

    TEST_CASE("bad cast") {
        int i = 42;
        shard::any a = i;

        CHECK_THROWS_AS(shard::any_cast<float>(a), shard::bad_any_cast);
    }

    TEST_CASE("reset method") {
        shard::any value = "foobar";

        CHECK(value.has_value());

        value.reset();
        CHECK_FALSE(value.has_value());

        value.reset();
        CHECK_FALSE(value.has_value());

        value = 42;
        CHECK(value.has_value());

        value.reset();
        CHECK_FALSE(value.has_value());
    }

    TEST_CASE("type with defined operator&") {
        int i = 42;
        const auto ptr = &i;
        test::addresser<int> addrress(ptr);
        shard::any a = addrress;

        auto returned = shard::any_cast<test::addresser<int>>(a);

        CHECK(&i == returned.ptr);
        CHECK(shard::any_cast<test::addresser<int>>(&a) != nullptr);
    }
}
