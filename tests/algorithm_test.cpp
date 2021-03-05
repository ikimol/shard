// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/algorithm.hpp>

#include <doctest.h>

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

TEST_SUITE("algorithm") {
    TEST_CASE("container_utils") {
        std::list<int> list;
        std::vector<int> vector;
        vector.reserve(5);
        for (int i = 0; i < 5; ++i) {
            list.emplace_back(i);
            vector.emplace_back(i);
        }

        SUBCASE("insert_ordered") {
            int expected[] = {0, 1, 2, 2, 3, 4};

            shard::insert_ordered(list, 2);
            shard::insert_ordered(vector, 2);

            auto it = list.begin();
            for (std::size_t i = 0; i < list.size(); ++i) {
                CHECK(expected[i] == *it);
                std::advance(it, 1);
            }

            for (std::size_t i = 0; i < vector.size(); ++i) {
                CHECK(vector[i] == expected[i]);
            }
        }

        SUBCASE("remove_unordered") {
            int expected[] = {0, 4, 2, 3};

            shard::remove_unordered(list, ++list.begin());
            shard::remove_unordered(vector, ++vector.begin());

            auto it = list.begin();
            for (std::size_t i = 0; i < list.size(); ++i) {
                CHECK(expected[i] == *it);
                std::advance(it, 1);
            }

            for (std::size_t i = 0; i < vector.size(); ++i) {
                CHECK(vector[i] == expected[i]);
            }
        }

        SUBCASE("has_key") {
            std::map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
            CHECK(shard::has_key(map, "one"));
            CHECK_FALSE(shard::has_key(map, "four"));

            std::set<std::string> set = {"foo", "bar", "baz"};
            CHECK(shard::has_key(set, "bar"));
            CHECK_FALSE(shard::has_key(set, "quack"));
        }

        SUBCASE("keys_of") {
            std::map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
            std::set<std::string> keys;
            shard::keys_of(map, std::inserter(keys, keys.end()));

            CHECK(keys.size() == 3);
            CHECK(shard::has_key(keys, "one"));
            CHECK(shard::has_key(keys, "two"));
            CHECK(shard::has_key(keys, "three"));
        }

        SUBCASE("values_of") {
            std::map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
            std::vector<int> values;
            shard::values_of(map, std::back_inserter(values));

            CHECK(values.size() == 3);
            CHECK(shard::contains(values, 1));
            CHECK(shard::contains(values, 2));
            CHECK(shard::contains(values, 3));
        }

        SUBCASE("erase") {
            shard::erase(list, 3);
            shard::erase(vector, 3);

            CHECK(list == std::list<int>({0, 1, 2, 4}));
            CHECK(vector == std::vector<int>({0, 1, 2, 4}));
        }
    }

    TEST_CASE("ipairs") {
        int array[5];
        for (int i = 0; i < 5; ++i) {
            array[i] = i * 2;
        }

        SUBCASE("array") {
            for (auto& item : shard::ipairs(array)) {
                CHECK(item.value() == item.index() * 2);
            }
        }

        SUBCASE("initializer_list") {
            for (auto& item : shard::ipairs(std::initializer_list<int> {0, 2, 4, 6, 8})) {
                CHECK(item.value() == item.index() * 2);
            }
        }

        SUBCASE("changing values") {
            for (auto& item : shard::ipairs(array)) {
                item.value() *= 2;
            }

            for (int i = 0; i < 5; ++i) {
                CHECK(array[i] == i * 4);
            }
        }
    }

    TEST_CASE("pairs") {
        SUBCASE("map") {
            std::map<int, int> map = {
            // clang-format off
                {0, 0},
                {1, 2},
                {2, 4},
                {3, 6},
                {4, 8},
            // clang-format on
            };

            for (auto& p : shard::pairs(map)) {
                CHECK(p.key() == p.index());
                CHECK(p.value() == p.key() * 2);
            }
        }

        SUBCASE("vector of pairs") {
            std::vector<std::pair<int, int>> vector = {
            // clang-format off
                {0, 0},
                {1, 2},
                {2, 4},
                {3, 6},
                {4, 8},
            // clang-format on
            };

            for (auto& p : shard::pairs(vector)) {
                CHECK(p.key() == p.index());
                CHECK(p.value() == p.key() * 2);
            }
        }
    }

    TEST_CASE("stl_wrappers") {
        int array[5];
        std::list<int> list;
        std::set<int> set;
        std::vector<int> vector;

        vector.reserve(5);
        for (int i = 0; i < 5; ++i) {
            array[i] = i;
            list.emplace_back(i);
            set.insert(i);
            vector.emplace_back(i);
        }

        SUBCASE("contains") {
            CHECK(shard::contains(array, 3));
            CHECK(shard::contains(list, 3));
            CHECK(shard::contains(set, 3));
            CHECK(shard::contains(vector, 3));
        }

        SUBCASE("index_of") {
            CHECK(shard::index_of(list, 1) == 1);
            CHECK(shard::index_of(vector, 1) == 1);
        }
    }
}
