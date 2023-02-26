// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include "helpers/counter.hpp"

#include <shard/containers/hash_map.hpp>
#include <shard/memory/allocators/heap_allocator.hpp>

#include <doctest.h>

#include <string>

struct dumb_int_hash {
    std::size_t operator()(int i) const { return i % 3; }
};

TEST_CASE("hash_map") {
    shard::heap_allocator allocator;

    SUBCASE("constructor") {
        shard::hash_map<int, std::string> map(allocator);

        REQUIRE(map.is_empty());
        REQUIRE(map.bucket_count() == 0);

        // verify underlying memory allocations
        REQUIRE(allocator.used_memory() == 0);
        REQUIRE(allocator.allocation_count() == 0);
    }

    SUBCASE("constructor with bucket count") {
        shard::hash_map<int, std::string> map(allocator, 5);

        REQUIRE(map.is_empty());
        REQUIRE(map.bucket_count() == 0);
        REQUIRE(map.capacity() == 5);

        // verify underlying memory allocations
        // a bucket is a pointer (i.e. 8 bytes)
        REQUIRE(allocator.used_memory() == 5 * sizeof(void*) + sizeof(std::size_t));
        REQUIRE(allocator.allocation_count() == 1);
    }

    SUBCASE("copy constructor") {
        // FIXME: Not implemented
    }

    SUBCASE("move constructor") {
        shard::hash_map<int, std::string> map(allocator);
        map.emplace(0, "foo");
        map.emplace(1, "bar");
        map.emplace(2, "baz");

        auto moved_to = std::move(map);

        REQUIRE(moved_to.size() == 3);

        REQUIRE(moved_to.get(0) == "foo");
        REQUIRE(moved_to.get(1) == "bar");
        REQUIRE(moved_to.get(2) == "baz");

        // TODO: Verify underlying memory allocations
    }

    SUBCASE("destructor") {
        test::counter::reset();
        {
            shard::hash_map<int, test::counter> array(allocator);
            array.emplace(0);
            REQUIRE(test::counter::default_constructor == 1);
        }
        REQUIRE(allocator.used_memory() == 0);
        REQUIRE(allocator.allocation_count() == 0);

        REQUIRE(test::counter::destructor == 1);
    }

    SUBCASE("copy assignment") {
        // FIXME: Not implemented
    }

    SUBCASE("move assignment") {
        // FIXME: Not implemented
    }

    SUBCASE("emplace") {
        shard::hash_map<int, std::string> map(allocator);

        map.emplace(0);
        REQUIRE(map.size() == 1);
        REQUIRE(map.contains(0));

        map.emplace(1, "foo");
        REQUIRE(map.size() == 2);
        REQUIRE(map.contains(1));
        REQUIRE(map.get(1) == "foo");

        SUBCASE("existing key") {
            REQUIRE(map.get(0).empty());
            auto pair = map.emplace(0, "bar");
            REQUIRE_FALSE(pair.second);
            REQUIRE(pair.first->value.empty());
            (*pair.first).value = "bar";
            REQUIRE(map.get(0) == "bar");
        }
    }

    SUBCASE("remove") {
        shard::hash_map<int, std::string> map(allocator);

        map.emplace(0, "foo");
        map.emplace(1, "bar");
        map.emplace(2, "baz");

        map.remove(1);
        REQUIRE_FALSE(map.contains(1));
        REQUIRE(map.size() == 2);
    }

    SUBCASE("clear") {
        shard::hash_map<int, std::string> map(allocator);

        map.emplace(0, "foo");
        map.emplace(1, "bar");
        map.emplace(2, "baz");

        map.clear();
        REQUIRE(map.is_empty());
        REQUIRE(map.capacity() > 0);
        REQUIRE(map.bucket_count() > 0);
    }

    SUBCASE("swap") {
        shard::hash_map<int, char> lhs(allocator);
        lhs.emplace(0, 'a');
        lhs.emplace(1, 'b');
        lhs.emplace(2, 'c');

        shard::hash_map<int, char> rhs(allocator);
        rhs.emplace(0, 'x');
        rhs.emplace(1, 'y');
        rhs.emplace(2, 'z');

        {
            using std::swap;
            swap(lhs, rhs);
        }

        REQUIRE(lhs.size() == rhs.size());
        REQUIRE(lhs[0] == 'x');
        REQUIRE(rhs[0] == 'a');

        REQUIRE(lhs[1] == 'y');
        REQUIRE(rhs[1] == 'b');

        REQUIRE(lhs[2] == 'z');
        REQUIRE(rhs[2] == 'c');
    }

    SUBCASE("get") {
        shard::hash_map<int, std::string> map(allocator);

        map.emplace(0, "foo");

        REQUIRE_NOTHROW(map.get(0));
        REQUIRE_THROWS_AS(map.get(1), std::out_of_range);

        REQUIRE(map.get(0) == "foo");
    }

    SUBCASE("operator[]") {
        shard::hash_map<int, std::string> map(allocator);

        map.emplace(0, "foo");

        REQUIRE_NOTHROW(map[0]);
        REQUIRE_NOTHROW(map[1]);

        REQUIRE(map[0] == "foo");

        auto& map_2 = map[2];
        REQUIRE(map_2.empty());

        map[3] = "bar";
        auto& map_3 = map[3];
        REQUIRE(map_3 == "bar");
    }

    SUBCASE("buckets") {
        shard::hash_map<int, std::string, dumb_int_hash> map(allocator);

        SUBCASE("bucket count") {
            REQUIRE(map.bucket_count() == 0);

            map.emplace(0, "foo");
            REQUIRE(map.bucket_count() == 2);

            map.emplace(1, "bar");
            REQUIRE(map.bucket_count() == 2);

            map.emplace(2, "baz");
            REQUIRE(map.bucket_count() == 5);
        }

        SUBCASE("getting a bucket") {
            map.emplace(0, "foo");
            REQUIRE(map.bucket_size(map.bucket(0)));

            map.emplace(3, "bar");
            map.emplace(6, "baz");
            map.emplace(7, "quack");

            REQUIRE(map.bucket_count() == 5);

            auto b0 = map.bucket(0);
            auto b1 = map.bucket(1);

            REQUIRE(map.bucket_size(b0) == 3);
            REQUIRE(map.bucket_size(b1) == 1);
        }
    }

    SUBCASE("hashing") {
        shard::hash_map<int, std::string, dumb_int_hash> map(allocator);

        REQUIRE(map.bucket_count() == 0);
        REQUIRE(map.load_factor() == 0.f);
        REQUIRE(map.max_load_factor() == 1.f);

        auto insert_and_require = [&](int key, const std::string& value, std::size_t count, float load) {
            map.emplace(key, value);
            REQUIRE(map.bucket_count() == count);
            REQUIRE(map.load_factor() == doctest::Approx(load));
        };

        SUBCASE("default max load factor") {
            insert_and_require(0, "foo", 2, 0.5f);
            insert_and_require(1, "bar", 2, 1.f);
            // -- map will be rehashed
            insert_and_require(2, "baz", 5, 0.6f);
        }

        SUBCASE("custom max load factor") {
            map.set_max_load_factor(2.f);
            REQUIRE(map.max_load_factor() == 2.f);
            insert_and_require(0, "foo", 2, 0.5f);
            insert_and_require(1, "bar", 2, 1.f);
            insert_and_require(2, "baz", 2, 1.5f);
            insert_and_require(3, "qux", 2, 2.f);
            // -- map will be rehashed
            insert_and_require(4, "waldo", 5, 1.f);
            insert_and_require(5, "thud", 5, 1.2f);
        }
    }

    SUBCASE("iterators") {
        shard::hash_map<int, std::string> map(allocator);
        map.emplace(0, "foo");
        map.emplace(1, "bar");
        map.emplace(2, "baz");

        shard::array<std::string> values(allocator);
        values.append("foo");
        values.append("bar");
        values.append("baz");

        SUBCASE("iterator") {
            auto it = map.begin();
            REQUIRE((*it).key == 0);
            std::advance(it, 1);
            REQUIRE((*it).value == "bar");
            std::advance(it, 2);
            REQUIRE(it == map.end());
        }

        SUBCASE("range-based for loop") {
            for (const auto& e : map) {
                REQUIRE(e.value == values[e.key]);
            }
        }
    }
}
