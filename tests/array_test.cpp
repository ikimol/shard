// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include "helpers/counter.hpp"
#include "helpers/widget.hpp"

#include <shard/containers/array.hpp>
#include <shard/memory/allocators/heap_allocator.hpp>

#include <doctest.h>

#include <initializer_list>
#include <string>
#include <vector>

TEST_CASE("array") {
    shard::heap_allocator allocator;

    SUBCASE("constructor") {
        shard::array<int> array(allocator);

        REQUIRE(array.is_empty());
        REQUIRE(array.capacity() == 0);

        // verify underlying memory allocations
        REQUIRE(allocator.used_memory() == 0);
        REQUIRE(allocator.allocation_count() == 0);
    }

    SUBCASE("constructor with capacity") {
        shard::array<int> array(allocator, 5);

        REQUIRE(array.is_empty());
        REQUIRE(array.capacity() == 5);

        // verify underlying memory allocations
        REQUIRE(allocator.used_memory() == 5 * sizeof(int) + sizeof(std::size_t));
        REQUIRE(allocator.allocation_count() == 1);
    }

    SUBCASE("constructor with iterators") {
        std::vector<int> vector = {0, 1, 2, 3, 4};
        shard::array<int> array(allocator, std::begin(vector), std::end(vector));

        REQUIRE(array.size() == 5);

        REQUIRE(array.at(0) == 0);
        REQUIRE(array.at(1) == 1);
        REQUIRE(array.at(2) == 2);
        REQUIRE(array.at(3) == 3);
        REQUIRE(array.at(4) == 4);

        // verify underlying memory allocations
        REQUIRE(allocator.used_memory() == 5 * sizeof(int) + sizeof(std::size_t));
        REQUIRE(allocator.allocation_count() == 1);
    }

    SUBCASE("constructor with initializer list") {
        shard::array<std::string> array(allocator, {"foo", "bar", "baz", "qux"});

        REQUIRE(array.size() == 4);

        REQUIRE(array.at(0) == "foo");
        REQUIRE(array.at(1) == "bar");
        REQUIRE(array.at(2) == "baz");
        REQUIRE(array.at(3) == "qux");

        // verify underlying memory allocations
        REQUIRE(allocator.used_memory() == 4 * sizeof(std::string) + sizeof(std::size_t));
        REQUIRE(allocator.allocation_count() == 1);
    }

    SUBCASE("copy constructor") {
        shard::array<int> array(allocator, {0, 1, 2});
        auto copy = array;

        REQUIRE(copy.size() == array.size());
        REQUIRE(copy.capacity() == array.capacity());

        REQUIRE(copy == array);
        REQUIRE(&copy[0] != &array[0]);
        REQUIRE(&copy[1] != &array[1]);
        REQUIRE(&copy[2] != &array[2]);

        // verify underlying memory allocations
        auto memory_for_one = 3 * sizeof(int) + sizeof(std::size_t);
        REQUIRE(allocator.used_memory() == memory_for_one * 2);
        REQUIRE(allocator.allocation_count() == 2);
    }

    SUBCASE("move constructor") {
        shard::array<int> array(allocator, {0, 1, 2});
        auto moved_to = std::move(array);

        REQUIRE(moved_to.size() == 3);
        REQUIRE(moved_to.capacity() == 3);

        REQUIRE(moved_to.at(0) == 0);
        REQUIRE(moved_to.at(1) == 1);
        REQUIRE(moved_to.at(2) == 2);

        // verify underlying memory allocations
        REQUIRE(allocator.used_memory() == 3 * sizeof(int) + sizeof(std::size_t));
        REQUIRE(allocator.allocation_count() == 1);
    }

    SUBCASE("destructor") {
        {
            shard::array<test::counter> array(allocator);
            array.emplace_back();
            REQUIRE(test::counter::default_constructor == 1);
        }
        REQUIRE(allocator.used_memory() == 0);
        REQUIRE(allocator.allocation_count() == 0);

        REQUIRE(test::counter::destructor == 1);
        test::counter::reset();
    }

    SUBCASE("copy assignment") {
        shard::array<int> copy(allocator);
        shard::array<int> array(allocator, {0, 1, 2});

        copy = array;

        REQUIRE(copy.size() == array.size());
        REQUIRE(copy.capacity() == array.capacity());

        REQUIRE(copy == array);
        REQUIRE(&copy[0] != &array[0]);
        REQUIRE(&copy[1] != &array[1]);
        REQUIRE(&copy[2] != &array[2]);

        // verify underlying memory allocations
        auto memory_for_one = 3 * sizeof(int) + sizeof(std::size_t);
        REQUIRE(allocator.used_memory() == memory_for_one * 2);
        REQUIRE(allocator.allocation_count() == 2);
    }

    SUBCASE("move assignment") {
        shard::array<int> moved_to(allocator);
        shard::array<int> array(allocator, {0, 1, 2});

        moved_to = std::move(array);

        REQUIRE(moved_to.size() == 3);
        REQUIRE(moved_to.capacity() == 3);

        REQUIRE(moved_to.at(0) == 0);
        REQUIRE(moved_to.at(1) == 1);
        REQUIRE(moved_to.at(2) == 2);

        // verify underlying memory allocations
        REQUIRE(allocator.used_memory() == 3 * sizeof(int) + sizeof(std::size_t));
        REQUIRE(allocator.allocation_count() == 1);
    }

    SUBCASE("append") {
        shard::array<int> array(allocator);

        int value = 42;
        array.append(value);
        array.append(21);

        REQUIRE(array.size() == 2);
        REQUIRE(array.capacity() == 4);
        REQUIRE(array[0] == 42);
        REQUIRE(array[1] == 21);
    }

    SUBCASE("insert") {
        shard::array<int> array(allocator);

        int value = 42;
        array.insert(std::begin(array), value);
        array.insert(std::begin(array), 21);

        REQUIRE(array.size() == 2);
        REQUIRE(array[0] == 21);
        REQUIRE(array[1] == 42);
    }

    SUBCASE("emplace") {
        shard::array<test::widget> array(allocator);

        array.emplace(std::begin(array), 3, 4);
        array.emplace(std::begin(array), 42);

        REQUIRE(array.size() == 2);
        REQUIRE(array[0].test(42, 42));
        REQUIRE(array[1].test(3, 4));
    }

    SUBCASE("emplace_back") {
        shard::array<test::widget> array(allocator);

        array.emplace_back(3, 4);
        REQUIRE(array[0].test(3, 4));

        array.emplace_back(42);
        REQUIRE(array[1].test(42, 42));
    }

    SUBCASE("remove") {
        SUBCASE("trivially destructible") {
            shard::array<int> array(allocator, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

            array.remove(array.begin() + 1);
            REQUIRE(array.size() == 9);
            REQUIRE(array.capacity() == 10);
            REQUIRE(array[1] == 2);

            array.remove(array.begin() + 2, array.end() - 2);
            REQUIRE(array.size() == 4);
            REQUIRE(array[0] == 0);
            REQUIRE(array[1] == 2);
            REQUIRE(array[2] == 8);
            REQUIRE(array[3] == 9);
        }

        SUBCASE("non-trivially destructible") {
            shard::array<test::counter> array(allocator);
            array.resize(10);
            REQUIRE(test::counter::default_constructor == 10);
            array.remove(array.begin(), array.end() - 5);
            REQUIRE(test::counter::destructor == 5);
        }

        test::counter::reset();
    }

    SUBCASE("remove_last") {
        shard::array<test::counter> array(allocator);
        array.emplace_back();
        REQUIRE_FALSE(array.is_empty());
        REQUIRE(test::counter::default_constructor == 1);
        array.remove_last();
        REQUIRE(array.is_empty());
        REQUIRE(test::counter::destructor == 1);
        test::counter::reset();
    }

    SUBCASE("clear") {
        shard::array<int> array(allocator, {0, 1, 2, 3, 4});

        REQUIRE(array.size() == 5);
        REQUIRE(array.capacity() == 5);
        array.clear();
        REQUIRE(array.is_empty());
        REQUIRE(array.capacity() == 5);
    }

    SUBCASE("swap") {
        shard::array<int> lhs(allocator, {0, 1, 2});
        shard::array<int> rhs(allocator, {5, 6, 7});

        {
            using std::swap;
            swap(lhs, rhs);
        }

        REQUIRE(lhs.size() == rhs.size());
        REQUIRE(lhs[0] == 5);
        REQUIRE(rhs[0] == 0);

        REQUIRE(lhs[1] == 6);
        REQUIRE(rhs[1] == 1);

        REQUIRE(lhs[2] == 7);
        REQUIRE(rhs[2] == 2);
    }

    SUBCASE("first and last") {
        shard::array<int> array(allocator, {0, 1, 2, 3, 4});

        REQUIRE(array.first() == 0);
        REQUIRE(array.last() == 4);
        array.emplace_back(5);
        REQUIRE(array.last() == 5);
        array.clear();
        REQUIRE_THROWS_AS(array.first(), std::out_of_range);
    }

    SUBCASE("at") {
        shard::array<int> array(allocator);

        REQUIRE_THROWS_AS(array.at(0), std::out_of_range);
        array.append(42);
        REQUIRE_NOTHROW(array.at(0));
    }

    SUBCASE("operator[]") {
        shard::array<int> array(allocator);

        REQUIRE_NOTHROW(array[0]);
        array.append(42);
        REQUIRE_NOTHROW(array[0]);
    }

    SUBCASE("reserve") {
        shard::array<int> array(allocator);

        REQUIRE(array.capacity() == 0);
        array.reserve(100);
        REQUIRE(array.capacity() == 100);

        // verify underlying memory allocations
        REQUIRE(allocator.used_memory() == 100 * sizeof(int) + sizeof(std::size_t));
        REQUIRE(allocator.allocation_count() == 1);
    }

    SUBCASE("resize") {
        shard::array<int> array(allocator);

        REQUIRE(array.is_empty());
        array.resize(10);
        REQUIRE(array.size() == 10);
        REQUIRE(array.capacity() == 16);

        // verify underlying memory allocations
        REQUIRE(allocator.used_memory() == 16 * sizeof(int) + sizeof(std::size_t));
        REQUIRE(allocator.allocation_count() == 1);
    }

    SUBCASE("shrink_to_fit") {
        shard::array<int> array(allocator);

        array.resize(10);
        REQUIRE(array.capacity() == 16);
        array.shrink_to_fit();
        REQUIRE(array.capacity() == 10);

        // verify underlying memory allocations
        REQUIRE(allocator.used_memory() == 10 * sizeof(int) + sizeof(std::size_t));
        REQUIRE(allocator.allocation_count() == 1);
    }
}
