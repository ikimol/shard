// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include "helpers/widget.hpp"

#include <shard/memory.hpp>

#include <doctest.h>

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

TEST_SUITE("memory") {
    TEST_CASE("allocators") {
        SUBCASE("free_list_allocator") {
            shard::free_list_allocator a(g_buffer, BUFFER_SIZE);
            CHECK(a.size() == BUFFER_SIZE);
            CHECK(a.used_memory() == 0);
            CHECK(a.allocation_count() == 0);

            auto w = shard::new_object<test::widget>(a, 3, 42);
            // 16 is the size of the allocation header
            CHECK(a.used_memory() == sizeof(test::widget) + 16);
            CHECK(a.allocation_count() == 1);

            CHECK(w->a == 3);
            CHECK(w->b == 42);

            shard::delete_object(a, w);
            CHECK(a.used_memory() == 0);
            CHECK(a.allocation_count() == 0);
        }

        SUBCASE("heap_allocator") {
            shard::heap_allocator a;
            CHECK(a.size() == 0);
            CHECK(a.used_memory() == 0);
            CHECK(a.allocation_count() == 0);

            auto w = shard::new_object<test::widget>(a, 3, 42);
            // 8 is the size of the allocation header
            CHECK(a.used_memory() == sizeof(test::widget) + 8);
            CHECK(a.allocation_count() == 1);

            CHECK(w->a == 3);
            CHECK(w->b == 42);

            shard::delete_object(a, w);
            CHECK(a.used_memory() == 0);
            CHECK(a.allocation_count() == 0);
        }

        SUBCASE("linear_allocator") {
            shard::linear_allocator a(g_buffer, BUFFER_SIZE);
            CHECK(a.size() == BUFFER_SIZE);
            CHECK(a.used_memory() == 0);
            CHECK(a.allocation_count() == 0);

            auto w = shard::new_object<test::widget>(a, 3, 42);
            CHECK(a.used_memory() == sizeof(test::widget));
            CHECK(a.allocation_count() == 1);

            CHECK(w->a == 3);
            CHECK(w->b == 42);

            {
                using namespace test;
                w->~widget();
            }
            a.clear();
            CHECK(a.used_memory() == 0);
            CHECK(a.allocation_count() == 0);
        }

        SUBCASE("pool_allocator") {
            shard::pool_allocator<test::widget> a(g_buffer, BUFFER_SIZE);
            CHECK(a.size() == BUFFER_SIZE);
            CHECK(a.used_memory() == 0);
            CHECK(a.allocation_count() == 0);

            auto w = shard::new_object<test::widget>(a, 3, 42);
            CHECK(a.used_memory() == sizeof(test::widget));
            CHECK(a.allocation_count() == 1);

            CHECK(w->a == 3);
            CHECK(w->b == 42);

            shard::delete_object(a, w);
            CHECK(a.used_memory() == 0);
            CHECK(a.allocation_count() == 0);
        }

        SUBCASE("proxy_allocator") {
            shard::pool_allocator<test::widget> a(g_buffer, BUFFER_SIZE);
            shard::proxy_allocator proxy(a, "test");
            CHECK(proxy.size() == BUFFER_SIZE);
            CHECK(proxy.used_memory() == 0);
            CHECK(proxy.allocation_count() == 0);
            CHECK(proxy.name() == std::string("test"));

            auto w = shard::new_object<test::widget>(proxy, 3, 42);
            CHECK(proxy.used_memory() == sizeof(test::widget));
            CHECK(proxy.allocation_count() == 1);

            CHECK(w->a == 3);
            CHECK(w->b == 42);

            shard::delete_object(proxy, w);
            CHECK(proxy.used_memory() == 0);
            CHECK(proxy.allocation_count() == 0);
        }

        SUBCASE("static_allocator") {
            shard::static_allocator<BUFFER_SIZE> a;
            CHECK(a.size() == BUFFER_SIZE);
            CHECK(a.used_memory() == 0);
            CHECK(a.allocation_count() == 0);

            auto w = shard::new_object<test::widget>(a, 3, 42);
            CHECK(a.used_memory() == sizeof(test::widget));
            CHECK(a.allocation_count() == 1);

            CHECK(w->a == 3);
            CHECK(w->b == 42);

            {
                using namespace test;
                w->~widget();
            }
            a.clear();
            CHECK(a.used_memory() == 0);
            CHECK(a.allocation_count() == 0);
        }
    }

    TEST_CASE("object") {
        SUBCASE("ref_count") {
            auto o = new test_class(42);
            CHECK(o->ref_count() == 0);
            o->retain();
            CHECK(o->ref_count() == 1);
            o->release(false);
            CHECK(o->ref_count() == 0);
            delete o;
        }

        SUBCASE("copy constructor") {
            auto o = new test_class(42);
            CHECK(o->ref_count() == 0);
            o->retain();
            CHECK(o->ref_count() == 1);

            auto copy = *o;
            CHECK(copy.ref_count() == 0);
            copy.retain();
            CHECK(copy.ref_count() == 1);

            o->release();
            CHECK(copy.ref_count() == 1);

            copy.release(false);
            CHECK(copy.ref_count() == 0);
        }

        SUBCASE("object_ptr") {
            shard::object_ptr<test_class> o(new test_class(42, "foo"));
            CHECK(o->ref_count() == 1);
            CHECK(o->id == 42);
            CHECK((*o).name == std::string("foo"));
            CHECK((bool)(o));

            SUBCASE("copying") {
                auto copied_to = o;
                CHECK(copied_to->ref_count() == 2);
                CHECK(o.get() != nullptr);
            }

            SUBCASE("moving") {
                auto moved_to = std::move(o);
                CHECK(moved_to->ref_count() == 1);
                CHECK(o.get() == nullptr); /* NOLINT */
            }
        }

        SUBCASE("make_object") {
            auto o = shard::make_object<test_class>(42, "foo");
            CHECK(o->id == 42);
            CHECK(o->name == std::string("foo"));
        }
    }

    TEST_CASE("utils") {
        auto p2 = shard::memory::add(g_buffer, 2);
        CHECK(p2 == g_buffer + 2);

        auto p1 = shard::memory::sub(p2, 1);
        CHECK(p1 == g_buffer + 1);

        SUBCASE("padding") {
            auto padding = shard::memory::get_padding(p1, 4);
            CHECK(padding == 3);

            SUBCASE("with bigger header") {
                struct header {
                    std::size_t size = 0;
                };

                auto padding_with_header = shard::memory::get_padding<header>(p1, 4);
                CHECK(padding_with_header == 15);
            }

            SUBCASE("with smaller header") {
                struct header {
                    short size = 0;
                };

                auto padding_with_header = shard::memory::get_padding<header>(p1, 4);
                CHECK(padding_with_header == 3);
            }
        }
    }
}