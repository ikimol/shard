// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include "helpers/widget.hpp"

#include <shard/memory.hpp>

#include <catch.hpp>

#include <string>

#define BUFFER_SIZE 512

// statically allocated buffer
static char g_buffer[BUFFER_SIZE];

struct test_class : public shard::object {
    test_class() = default;

    explicit test_class(int id) : test_class(id, "") {}

    test_class(int id, const char* name) : id(id), name(name) {}

    int id = 0;
    const char* name = "";
};

TEST_CASE("memory") {
    SECTION("allocators") {
        SECTION("free_list_allocator") {
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

        SECTION("heap_allocator") {
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

        SECTION("linear_allocator") {
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

        SECTION("pool_allocator") {
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

        SECTION("proxy_allocator") {
            shard::pool_allocator<test::widget> a(g_buffer, BUFFER_SIZE);
            shard::proxy_allocator proxy(a, "test");
            REQUIRE(proxy.size() == BUFFER_SIZE);
            REQUIRE(proxy.used_memory() == 0);
            REQUIRE(proxy.allocation_count() == 0);
            CHECK_THAT(proxy.name(), Catch::Matches("test"));

            auto w = shard::new_object<test::widget>(proxy, 3, 42);
            REQUIRE(proxy.used_memory() == sizeof(test::widget));
            REQUIRE(proxy.allocation_count() == 1);

            REQUIRE(w->a == 3);
            REQUIRE(w->b == 42);

            shard::delete_object(proxy, w);
            REQUIRE(proxy.used_memory() == 0);
            REQUIRE(proxy.allocation_count() == 0);
        }

        SECTION("static_allocator") {
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

    SECTION("object") {
        SECTION("ref_count") {
            auto o = new test_class(42);
            REQUIRE(o->ref_count() == 0);
            o->retain();
            REQUIRE(o->ref_count() == 1);
            o->release(false);
            REQUIRE(o->ref_count() == 0);
            delete o;
        }

        SECTION("copy constructor") {
            auto o = new test_class(42);
            REQUIRE(o->ref_count() == 0);
            o->retain();
            REQUIRE(o->ref_count() == 1);

            auto copy = *o;
            REQUIRE(copy.ref_count() == 0);
            copy.retain();
            REQUIRE(copy.ref_count() == 1);

            o->release();
            REQUIRE(copy.ref_count() == 1);

            copy.release(false);
            REQUIRE(copy.ref_count() == 0);
        }

        SECTION("object_ptr") {
            shard::object_ptr<test_class> o(new test_class(42, "foo"));
            REQUIRE(o->ref_count() == 1);
            REQUIRE(o->id == 42);
            CHECK_THAT((*o).name, Catch::Matches("foo"));
            REQUIRE((bool)(o));

            SECTION("copying") {
                auto copied_to = o;
                REQUIRE(copied_to->ref_count() == 2);
                REQUIRE(o.get() != nullptr);
            }

            SECTION("moving") {
                auto moved_to = std::move(o);
                REQUIRE(moved_to->ref_count() == 1);
                REQUIRE(o.get() == nullptr); /* NOLINT */
            }
        }

        SECTION("make_object") {
            auto o = shard::make_object<test_class>(42, "foo");
            REQUIRE(o->id == 42);
            CHECK_THAT(o->name, Catch::Matches("foo"));
        }
    }

    SECTION("utils") {
        auto p2 = shard::memory::add(g_buffer, 2);
        REQUIRE(p2 == g_buffer + 2);

        auto p1 = shard::memory::sub(p2, 1);
        REQUIRE(p1 == g_buffer + 1);

        SECTION("padding") {
            auto padding = shard::memory::get_padding(p1, 4);
            REQUIRE(padding == 3);

            SECTION("with bigger header") {
                struct header {
                    std::size_t size = 0;
                };

                auto padding_with_header = shard::memory::get_padding<header>(p1, 4);
                REQUIRE(padding_with_header == 15);
            }

            SECTION("with smaller header") {
                struct header {
                    short size = 0;
                };

                auto padding_with_header = shard::memory::get_padding<header>(p1, 4);
                REQUIRE(padding_with_header == 3);
            }
        }
    }
}