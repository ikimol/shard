// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/algorithm.hpp>

#include <doctest.h>

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

TEST_CASE("algorithm") {
    SUBCASE("container_utils") {
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

        SUBCASE("get_value") {
            std::map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
            auto value = shard::get_value(map, "one");
            REQUIRE(value);
            REQUIRE(value.value() == 1);

            value = shard::get_value(map, "four");
            REQUIRE_FALSE(value);
        }

        SUBCASE("contains") {
            std::map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
            REQUIRE(shard::contains(map, "one"));
            REQUIRE_FALSE(shard::contains(map, "four"));

            std::set<std::string> set = {"foo", "bar", "baz"};
            REQUIRE(shard::contains(set, "bar"));
            REQUIRE_FALSE(shard::contains(set, "quack"));
        }

        SUBCASE("keys_of") {
            std::map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
            std::set<std::string> keys;
            shard::keys_of(map, std::inserter(keys, keys.end()));

            REQUIRE(keys.size() == 3);
            REQUIRE(shard::contains(keys, "one"));
            REQUIRE(shard::contains(keys, "two"));
            REQUIRE(shard::contains(keys, "three"));
        }

        SUBCASE("values_of") {
            std::map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
            std::vector<int> values;
            shard::values_of(map, std::back_inserter(values));

            REQUIRE(values.size() == 3);
            REQUIRE(shard::index_of(values, 1).has_value());
            REQUIRE(shard::index_of(values, 2).has_value());
            REQUIRE(shard::index_of(values, 3).has_value());
        }

        SUBCASE("erase") {
            shard::erase(list, 3);
            shard::erase(vector, 3);

            REQUIRE(list == std::list<int>({0, 1, 2, 4}));
            REQUIRE(vector == std::vector<int>({0, 1, 2, 4}));
        }
    }

    SUBCASE("enumerate") {
        int array[5];
        for (int i = 0; i < 5; ++i) {
            array[i] = i * 2;
        }

        SUBCASE("array") {
            for (auto& item : shard::enumerate(array)) {
                CHECK(item.value() == item.index() * 2);
            }
        }

        SUBCASE("initializer_list") {
            auto il = {0, 2, 4, 6, 8};
            for (auto& item : shard::enumerate(il)) {
                CHECK(item.value() == item.index() * 2);
            }
        }

        SUBCASE("rvalue") {
            for (auto& item : shard::enumerate(std::vector<int> {0, 2, 4, 6, 8, 10})) {
                CHECK(item.value() == item.index() * 2);
            }
        }

        SUBCASE("changing values") {
            for (auto& item : shard::enumerate(array)) {
                item.value() *= 2;
            }

            for (int i = 0; i < 5; ++i) {
                CHECK(array[i] == i * 4);
            }
        }
    }

    SUBCASE("stl_wrappers") {
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

        SUBCASE("index_of") {
            REQUIRE(shard::index_of(list, 1).has_value());
            REQUIRE(shard::index_of(list, 1).value() == 1);
            REQUIRE(shard::index_of(vector, 1).has_value());
            REQUIRE(shard::index_of(vector, 1).value() == 1);
        }
    }
}
