// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/algorithm.hpp>

#include <catch.hpp>

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

TEST_CASE("algorithm") {
    SECTION("container_utils") {
        std::list<int> list;
        std::vector<int> vector;
        vector.reserve(5);
        for (int i = 0; i < 5; ++i) {
            list.emplace_back(i);
            vector.emplace_back(i);
        }

        SECTION("insert_ordered") {
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

        SECTION("remove_unordered") {
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

        SECTION("has_key") {
            std::map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
            REQUIRE(shard::has_key(map, "one"));
            REQUIRE_FALSE(shard::has_key(map, "four"));

            std::set<std::string> set = {"foo", "bar", "baz"};
            REQUIRE(shard::has_key(set, "bar"));
            REQUIRE_FALSE(shard::has_key(set, "quack"));
        }

        SECTION("keys_of") {
            std::map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
            std::set<std::string> keys;
            shard::keys_of(map, std::inserter(keys, keys.end()));

            REQUIRE(keys.size() == 3);
            REQUIRE(shard::has_key(keys, "one"));
            REQUIRE(shard::has_key(keys, "two"));
            REQUIRE(shard::has_key(keys, "three"));
        }

        SECTION("values_of") {
            std::map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
            std::vector<int> values;
            shard::values_of(map, std::back_inserter(values));

            REQUIRE(values.size() == 3);
            REQUIRE(shard::contains(values, 1));
            REQUIRE(shard::contains(values, 2));
            REQUIRE(shard::contains(values, 3));
        }

        SECTION("erase") {
            shard::erase(list, 3);
            shard::erase(vector, 3);

            REQUIRE(list == std::list<int>({0, 1, 2, 4}));
            REQUIRE(vector == std::vector<int>({0, 1, 2, 4}));
        }
    }

    SECTION("enumerate") {
        int array[5];
        for (int i = 0; i < 5; ++i) {
            array[i] = i * 2;
        }

        SECTION("array") {
            for (auto& item : shard::enumerate(array)) {
                CHECK(item.value() == item.index() * 2);
            }
        }

        SECTION("initializer_list") {
            auto il = {0, 2, 4, 6, 8};
            for (auto& item : shard::enumerate(il)) {
                CHECK(item.value() == item.index() * 2);
            }
        }

        SECTION("rvalue") {
            for (auto& item : shard::enumerate(std::vector<int> {0, 2, 4, 6, 8, 10})) {
                CHECK(item.value() == item.index() * 2);
            }
        }

        SECTION("changing values") {
            for (auto& item : shard::enumerate(array)) {
                item.value() *= 2;
            }

            for (int i = 0; i < 5; ++i) {
                CHECK(array[i] == i * 4);
            }
        }
    }

    SECTION("stl_wrappers") {
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

        SECTION("contains") {
            REQUIRE(shard::contains(array, 3));
            REQUIRE(shard::contains(list, 3));
            REQUIRE(shard::contains(set, 3));
            REQUIRE(shard::contains(vector, 3));
        }

        SECTION("index_of") {
            REQUIRE(shard::index_of(list, 1) == 1);
            REQUIRE(shard::index_of(vector, 1) == 1);
        }
    }
}
