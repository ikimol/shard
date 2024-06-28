// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include "helpers/widget.hpp"

#include <shard/alloc/allocators/free_list_allocator.hpp>
#include <shard/alloc/allocators/heap_allocator.hpp>
#include <shard/alloc/allocators/linear_allocator.hpp>
#include <shard/alloc/allocators/pool_allocator.hpp>
#include <shard/alloc/allocators/proxy_allocator.hpp>
#include <shard/alloc/allocators/static_allocator.hpp>

#include <doctest.h>

#define BUFFER_SIZE 512

// statically allocated buffer
static char g_buffer[BUFFER_SIZE];

TEST_CASE("alloc") {
    SUBCASE("allocators") {
        SUBCASE("free_list_allocator") {
            shard::free_list_allocator a(g_buffer, BUFFER_SIZE);
            REQUIRE(a.size() == BUFFER_SIZE);
            REQUIRE(a.used_memory() == 0);
            REQUIRE(a.allocation_count() == 0);

            auto w = shard::new_object<test::widget>(a, 3, 42);
            // 16 is the size of the allocation header
            REQUIRE(a.used_memory() == sizeof(test::widget) + 16);
            REQUIRE(a.allocation_count() == 1);

            REQUIRE(w->a == 3);
            REQUIRE(w->b == 42);

            shard::delete_object(a, w);
            REQUIRE(a.used_memory() == 0);
            REQUIRE(a.allocation_count() == 0);
        }

        SUBCASE("heap_allocator") {
            shard::heap_allocator a;
            REQUIRE(a.size() == 0);
            REQUIRE(a.used_memory() == 0);
            REQUIRE(a.allocation_count() == 0);

            auto w = shard::new_object<test::widget>(a, 3, 42);
            // 8 is the size of the allocation header
            REQUIRE(a.used_memory() == sizeof(test::widget) + 8);
            REQUIRE(a.allocation_count() == 1);

            REQUIRE(w->a == 3);
            REQUIRE(w->b == 42);

            shard::delete_object(a, w);
            REQUIRE(a.used_memory() == 0);
            REQUIRE(a.allocation_count() == 0);
        }

        SUBCASE("linear_allocator") {
            shard::linear_allocator a(g_buffer, BUFFER_SIZE);
            REQUIRE(a.size() == BUFFER_SIZE);
            REQUIRE(a.used_memory() == 0);
            REQUIRE(a.allocation_count() == 0);

            auto w = shard::new_object<test::widget>(a, 3, 42);
            REQUIRE(a.used_memory() == sizeof(test::widget));
            REQUIRE(a.allocation_count() == 1);

            REQUIRE(w->a == 3);
            REQUIRE(w->b == 42);

            {
                using namespace test;
                w->~widget();
            }
            a.clear();
            REQUIRE(a.used_memory() == 0);
            REQUIRE(a.allocation_count() == 0);
        }

        SUBCASE("pool_allocator") {
            shard::pool_allocator<test::widget> a(g_buffer, BUFFER_SIZE);
            REQUIRE(a.size() == BUFFER_SIZE);
            REQUIRE(a.used_memory() == 0);
            REQUIRE(a.allocation_count() == 0);

            auto w = shard::new_object<test::widget>(a, 3, 42);
            REQUIRE(a.used_memory() == sizeof(test::widget));
            REQUIRE(a.allocation_count() == 1);

            REQUIRE(w->a == 3);
            REQUIRE(w->b == 42);

            shard::delete_object(a, w);
            REQUIRE(a.used_memory() == 0);
            REQUIRE(a.allocation_count() == 0);
        }

        SUBCASE("proxy_allocator") {
            shard::pool_allocator<test::widget> a(g_buffer, BUFFER_SIZE);
            shard::proxy_allocator proxy(a, "test");
            REQUIRE(proxy.size() == BUFFER_SIZE);
            REQUIRE(proxy.used_memory() == 0);
            REQUIRE(proxy.allocation_count() == 0);
            REQUIRE(proxy.name() == doctest::String("test"));

            auto w = shard::new_object<test::widget>(proxy, 3, 42);
            REQUIRE(proxy.used_memory() == sizeof(test::widget));
            REQUIRE(proxy.allocation_count() == 1);

            REQUIRE(w->a == 3);
            REQUIRE(w->b == 42);

            shard::delete_object(proxy, w);
            REQUIRE(proxy.used_memory() == 0);
            REQUIRE(proxy.allocation_count() == 0);
        }

        SUBCASE("static_allocator") {
            shard::static_allocator<BUFFER_SIZE> a;
            REQUIRE(a.size() == BUFFER_SIZE);
            REQUIRE(a.used_memory() == 0);
            REQUIRE(a.allocation_count() == 0);

            auto w = shard::new_object<test::widget>(a, 3, 42);
            REQUIRE(a.used_memory() == sizeof(test::widget));
            REQUIRE(a.allocation_count() == 1);

            REQUIRE(w->a == 3);
            REQUIRE(w->b == 42);

            {
                using namespace test;
                w->~widget();
            }
            a.clear();
            REQUIRE(a.used_memory() == 0);
            REQUIRE(a.allocation_count() == 0);
        }
    }
}
