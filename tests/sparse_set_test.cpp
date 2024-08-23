// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include "helpers/widget.hpp"

#include <shard/sparse_set.hpp>

#include <doctest.h>

template <>
struct shard::sparse_set_value_translator<test::widget> {
    static std::size_t translate(const test::widget& w) { return static_cast<std::size_t>(w.a); }
};

TEST_CASE("sparse_set") {
    SUBCASE("default constructor") {
        shard::sparse_set<unsigned> s;

        REQUIRE(s.is_empty());
        REQUIRE(s.capacity() == 0);

        REQUIRE_FALSE(s.contains(0));
    }

    SUBCASE("insert") {
        shard::sparse_set<unsigned> s;

        s.insert(0);

        REQUIRE(s.size() == 1);
        REQUIRE(s.capacity() == 4); // min capacity

        for (int i = 1; i < 10; ++i) {
            s.insert(i);
        }

        REQUIRE(s.size() == 10);
        REQUIRE(s.capacity() == 16);
    }

    SUBCASE("translator") {
        shard::sparse_set<test::widget> s;
        s.insert(test::widget {0});
        s.insert(test::widget {1});
        s.insert(test::widget {2});

        REQUIRE(s.contains(test::widget {0}));
        REQUIRE(s.contains(test::widget {1}));
        REQUIRE(s.contains(test::widget {2}));

        REQUIRE_FALSE(s.contains(test::widget {3}));
    }
}
