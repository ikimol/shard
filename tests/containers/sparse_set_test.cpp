// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include "helpers/widget.hpp"

#include <shard/sparse_set.hpp>

#include <doctest.h>

TEST_CASE("sparse_set") {
    SUBCASE("default constructor") {
        shard::sparse_set<unsigned> set;

        REQUIRE(set.is_empty());
        REQUIRE(set.capacity() == 0);
    }

    SUBCASE("copy constructor") {
        shard::sparse_set<unsigned> set;
        set.insert(0);
        set.insert(1);
        auto copy = set;

        REQUIRE(copy.size() == set.size());
        REQUIRE(copy.capacity() == set.capacity());

        // verify copy
        REQUIRE(copy.contains(0));
        REQUIRE(copy.contains(1));

        // verify original
        REQUIRE(set.contains(0));
        REQUIRE(set.contains(1));

        copy.insert(2);
        REQUIRE(copy.contains(2));
        REQUIRE_FALSE(set.contains(2));
    }

    SUBCASE("move constructor") {
        shard::sparse_set<unsigned> set;
        set.insert(0);
        set.insert(1);
        auto moved_to = std::move(set);

        // verify moved to
        REQUIRE(moved_to.size() == 2);
        REQUIRE(moved_to.capacity() == 4);

        // verify original
        REQUIRE(set.is_empty());
        REQUIRE(set.capacity() == 0);

        moved_to.insert(2);
        REQUIRE(moved_to.contains(2));
        REQUIRE(set.is_empty());
    }

    SUBCASE("copy assignment") {
        shard::sparse_set<unsigned> copy;
        shard::sparse_set<unsigned> set;
        set.insert(0);
        set.insert(1);

        copy = set;

        REQUIRE(copy.size() == set.size());
        REQUIRE(copy.capacity() == set.capacity());

        // verify copy
        REQUIRE(copy.contains(0));
        REQUIRE(copy.contains(1));

        // verify original
        REQUIRE(set.contains(0));
        REQUIRE(set.contains(1));

        copy.insert(2);
        REQUIRE(copy.contains(2));
        REQUIRE_FALSE(set.contains(2));
    }

    SUBCASE("move assignment") {
        shard::sparse_set<unsigned> moved_to;
        shard::sparse_set<unsigned> set;
        set.insert(0);
        set.insert(1);

        moved_to = std::move(set);

        // verify moved to
        REQUIRE(moved_to.size() == 2);
        REQUIRE(moved_to.capacity() == 4);

        // verify original
        REQUIRE(set.is_empty());
        REQUIRE(set.capacity() == 0);

        moved_to.insert(2);
        REQUIRE(moved_to.contains(2));
        REQUIRE(set.is_empty());
    }

    SUBCASE("insert") {
        shard::sparse_set<unsigned> set;
        set.insert(0);

        REQUIRE(set.size() == 1);
        REQUIRE(set.capacity() == 4); // min capacity

        for (int i = 1; i < 10; ++i) {
            set.insert(i);
        }

        REQUIRE(set.size() == 10);
        REQUIRE(set.capacity() == 16);
    }

    SUBCASE("erase") {
        shard::sparse_set<unsigned> set;
        set.insert(0);
        set.erase(0);

        REQUIRE(set.is_empty());
    }

    SUBCASE("contains") {
        shard::sparse_set<unsigned> set;
        set.insert(0);

        REQUIRE(set.contains(0));
        REQUIRE_FALSE(set.contains(1));

        set.erase(0);
        REQUIRE_FALSE(set.contains(0));
    }

    SUBCASE("index_of") {
        shard::sparse_set<unsigned> set;
        set.insert(21);
        set.insert(42);
        set.insert(84);

        REQUIRE(set.index_of(21) == 0);
        REQUIRE(set.index_of(42) == 1);

        set.erase(21); // swaps with last

        REQUIRE(set.index_of(42) == 1);
        REQUIRE(set.index_of(84) == 0);
    }

    SUBCASE("iterators") {
        shard::sparse_set<unsigned> set;
        set.insert(0);
        set.insert(1);

        auto it = set.begin();
        REQUIRE(*it++ == 0);
        REQUIRE(*it++ == 1);
        REQUIRE(it == set.end());
    }
}
