// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include "helpers/addresser.hpp"
#include "helpers/counter.hpp"

#include <shard/any.hpp>

#include <catch.hpp>

#include <string>

TEST_CASE("any") {
    SECTION("default constructor") {
        shard::any a;

        REQUIRE_FALSE(a.has_value());
        REQUIRE(shard::any_cast<int>(&a) == nullptr);
        REQUIRE(a.type() == typeid(void));
    }

    SECTION("copy constructor") {
        std::string s = "foobar";
        shard::any original = s;
        shard::any copy = original;

        REQUIRE(copy.has_value());
        REQUIRE(original.type() == copy.type());
        REQUIRE(shard::any_cast<std::string>(copy) == shard::any_cast<std::string>(original));
        REQUIRE(shard::any_cast<std::string>(copy) == s);
        REQUIRE(shard::any_cast<std::string>(&original) != shard::any_cast<std::string>(&copy));
    }

    SECTION("move constructor") {
        shard::any original(test::counter {});
        test::counter::reset();

        shard::any moved_to(std::move(original));

        REQUIRE_FALSE(original.has_value()); /* NOLINT */
        REQUIRE(moved_to.has_value());
        REQUIRE(moved_to.type() == typeid(test::counter));
        REQUIRE(shard::any_cast<test::counter>(&original) == nullptr);
        REQUIRE(shard::any_cast<test::counter>(&moved_to) != nullptr);
        REQUIRE(test::counter::copy_constructor == 0);
        REQUIRE(test::counter::move_constructor == 0);

        test::counter::reset();
    }

    SECTION("forwarding constructor") {
        std::string s = "foobar";
        shard::any a = s;

        REQUIRE(a.has_value());
        REQUIRE(a.type() == typeid(std::string));
        REQUIRE(shard::any_cast<int>(&a) == nullptr);
        REQUIRE(shard::any_cast<std::string>(&a) != nullptr);
        REQUIRE(shard::any_cast<std::string>(a) == s);
        REQUIRE(shard::any_cast<std::string>(&a) != &s);
    }

    SECTION("copy assignment") {
        std::string s = "foobar";
        shard::any original = s;
        shard::any copy;
        shard::any* result = &(copy = original);

        REQUIRE(copy.has_value());
        REQUIRE(original.type() == copy.type());
        REQUIRE(shard::any_cast<std::string>(copy) == shard::any_cast<std::string>(original));
        REQUIRE(shard::any_cast<std::string>(copy) == s);
        REQUIRE(shard::any_cast<std::string>(&original) != shard::any_cast<std::string>(&copy));
        REQUIRE(&copy == result);
    }

    SECTION("move assignment") {
        shard::any original = test::counter();
        shard::any moved_to;
        test::counter::reset();
        moved_to = std::move(original);

        REQUIRE_FALSE(original.has_value()); /* NOLINT */
        REQUIRE(moved_to.has_value());
        REQUIRE(moved_to.type() == typeid(test::counter));
        REQUIRE(shard::any_cast<test::counter>(&moved_to) != nullptr);
        REQUIRE(test::counter::copy_constructor == 0);
        REQUIRE(test::counter::move_constructor == 0);

        test::counter::reset();
    }

    SECTION("forwarding assignment") {
        std::string s = "foobar";
        shard::any a;
        shard::any* result = &(a = s);

        REQUIRE(a.has_value());
        REQUIRE(a.type() == typeid(std::string));
        REQUIRE(shard::any_cast<int>(&a) == nullptr);
        REQUIRE(shard::any_cast<std::string>(&a) != nullptr);
        REQUIRE(shard::any_cast<std::string>(a) == s);
        REQUIRE(shard::any_cast<std::string>(&a) != &s);
        REQUIRE(result == &a);
    }

    SECTION("emplace") {
        shard::any a;
        auto& s = a.emplace<std::string>("foobar");

        REQUIRE(a.has_value());
        REQUIRE(a.type() == typeid(std::string));
        REQUIRE(shard::any_cast<int>(&a) == nullptr);
        REQUIRE(shard::any_cast<std::string>(&a) != nullptr);
        REQUIRE(shard::any_cast<std::string>(a) == s);
        REQUIRE(shard::any_cast<std::string>(&a) == &s);

        // assign temporary

        test::counter::reset();
        {
            shard::any tmp = test::counter();
            REQUIRE(test::counter::default_constructor == 1);
            REQUIRE(test::counter::copy_constructor == 0);
            REQUIRE(test::counter::move_constructor == 1);
        }
        REQUIRE(test::counter::destructor == 2);

        // emplace type

        test::counter::reset();
        {
            shard::any tmp;
            tmp.emplace<test::counter>();
            REQUIRE(test::counter::default_constructor == 1);
            REQUIRE(test::counter::copy_constructor == 0);
            REQUIRE(test::counter::move_constructor == 0);
        }
        REQUIRE(test::counter::destructor == 1);
        test::counter::reset();
    }

    SECTION("swap") {
        std::string s = "foobar";
        shard::any original = s;
        shard::any swapped;
        auto original_ptr = shard::any_cast<std::string>(&original);
        original.swap(swapped);
        shard::any* result = &original;

        REQUIRE_FALSE(original.has_value());
        REQUIRE(swapped.has_value());
        REQUIRE(swapped.type() == typeid(std::string));
        REQUIRE(shard::any_cast<std::string>(swapped) == s);
        REQUIRE(original_ptr != nullptr);
        REQUIRE(original_ptr == shard::any_cast<std::string>(&swapped));
        REQUIRE(result == &original);

        test::counter::reset();
        {
            shard::any copy1 = test::counter();
            shard::any copy2 = test::counter();
            auto count = test::counter::copy_constructor;
            swap(copy1, copy2);
            REQUIRE(count == test::counter::copy_constructor);
        }
        test::counter::reset();
    }

    SECTION("cast to reference types") {
        shard::any a = 0;
        const shard::any b = a;

        int& a_ref = shard::any_cast<int&>(a);
        const int& a_cref = shard::any_cast<const int&>(a);

        REQUIRE(&a_ref == &a_cref);

        const int& b_cref = shard::any_cast<const int&>(b);

        REQUIRE(&a_ref != &b_cref);

        ++a_ref;
        int incremented = shard::any_cast<int>(a);

        REQUIRE(incremented == 1);

        REQUIRE_THROWS_AS(shard::any_cast<char&>(a), shard::bad_any_cast);
        REQUIRE_THROWS_AS(shard::any_cast<const char&>(b), shard::bad_any_cast);
    }

    SECTION("bad cast") {
        int i = 42;
        shard::any a = i;

        REQUIRE_THROWS_AS(shard::any_cast<float>(a), shard::bad_any_cast);
    }

    SECTION("reset method") {
        shard::any value = "foobar";

        REQUIRE(value.has_value());

        value.reset();
        REQUIRE_FALSE(value.has_value());

        value.reset();
        REQUIRE_FALSE(value.has_value());

        value = 42;
        REQUIRE(value.has_value());

        value.reset();
        REQUIRE_FALSE(value.has_value());
    }

    SECTION("type with defined operator&") {
        int i = 42;
        const auto ptr = &i;
        test::addresser<int> addrress(ptr);
        shard::any a = addrress;

        auto returned = shard::any_cast<test::addresser<int>>(a);

        REQUIRE(&i == returned.ptr);
        REQUIRE(shard::any_cast<test::addresser<int>>(&a) != nullptr);
    }
}
