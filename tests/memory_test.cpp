// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include "helpers/counter.hpp"
#include "helpers/widget.hpp"

#include <shard/memory.hpp>

#include <doctest.h>

#include <cstdint>

#define BUFFER_SIZE 512

// statically allocated buffer
static char g_buffer[BUFFER_SIZE];

// size to make allocations fail
static std::size_t g_too_much = PTRDIFF_MAX;

struct test_class : public shard::object {
    test_class() = default;

    explicit test_class(int id)
    : test_class(id, "") {}

    test_class(int id, const char* name)
    : id(id)
    , name(name) {}

    int id = 0;
    const char* name = "";
};

TEST_CASE("memory") {
    SUBCASE("buffer_ptr") {
        constexpr auto size = sizeof(test::widget);
        std::byte buffer[size];

        SUBCASE("constructor") {
            SUBCASE("default") {
                shard::buffer_ptr<test::widget> w(buffer);
                REQUIRE(w->a == 0);
                REQUIRE(w->b == 0);
            }

            SUBCASE("single argument") {
                shard::buffer_ptr<test::widget> w(buffer, 42);
                REQUIRE(w->a == 42);
                REQUIRE(w->b == 42);
            }

            SUBCASE("multiple arguments") {
                shard::buffer_ptr<test::widget> w(buffer, 3, 4);
                REQUIRE(w->a == 3);
                REQUIRE(w->b == 4);
            }
        }

        SUBCASE("destructor") {
            test::counter::reset();
            {
                shard::buffer_ptr<test::counter> w(buffer);
                REQUIRE(test::counter::default_constructor == 1);
            }
            REQUIRE(test::counter::destructor == 1);
        }
    }

    SUBCASE("object") {
        SUBCASE("ref_count") {
            auto o = new test_class(42);
            REQUIRE(o->ref_count() == 0);
            o->retain();
            REQUIRE(o->ref_count() == 1);
            o->release(false);
            REQUIRE(o->ref_count() == 0);
            delete o;
        }

        SUBCASE("copy constructor") {
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

        SUBCASE("object_ptr") {
            shard::object_ptr<test_class> o(new test_class(42, "foo"));
            REQUIRE(o->ref_count() == 1);
            REQUIRE(o->id == 42);
            REQUIRE((*o).name == doctest::String("foo"));
            REQUIRE((bool)(o));

            SUBCASE("copying") {
                auto copied_to = o;
                REQUIRE(copied_to->ref_count() == 2);
                REQUIRE(o.get() != nullptr);
            }

            SUBCASE("moving") {
                auto moved_to = std::move(o);
                REQUIRE(moved_to->ref_count() == 1);
                REQUIRE(o.get() == nullptr); /* NOLINT */
            }
        }

        SUBCASE("make_object") {
            auto o = shard::make_object<test_class>(42, "foo");
            REQUIRE(o->id == 42);
            REQUIRE(o->name == doctest::String("foo"));
        }
    }

    SUBCASE("utils") {
        auto p2 = shard::memory::add(g_buffer, 2);
        REQUIRE(p2 == g_buffer + 2);

        auto p1 = shard::memory::sub(p2, 1);
        REQUIRE(p1 == g_buffer + 1);

        SUBCASE("padding") {
            auto padding = shard::memory::get_padding(p1, 4);
            REQUIRE(padding == 3);

            SUBCASE("with bigger header") {
                struct header {
                    std::size_t size = 0;
                };

                auto padding_with_header = shard::memory::get_padding<header>(p1, 4);
                REQUIRE(padding_with_header == 15);
            }

            SUBCASE("with smaller header") {
                struct header {
                    short size = 0;
                };

                auto padding_with_header = shard::memory::get_padding<header>(p1, 4);
                REQUIRE(padding_with_header == 3);
            }
        }
    }

    SUBCASE("dynamic_data") {
        SUBCASE("default constructor") {
            shard::dynamic_data data;

            REQUIRE(data.size() == 0);
            REQUIRE(data.bytes() == nullptr);

            SUBCASE("release") {
                auto old_bytes = data.bytes();
                auto bytes = data.release();

                REQUIRE(old_bytes == bytes);
                REQUIRE(bytes == nullptr);
                REQUIRE(data.size() == 0);
                REQUIRE(data.bytes() == nullptr);
            }

            SUBCASE("reallocate") {
                data.reallocate(128);

                REQUIRE(data.size() == 128);
                REQUIRE(data.bytes() != nullptr);
            }
        }

        SUBCASE("construction with size") {
            shard::dynamic_data data(128);

            REQUIRE(data.size() == 128);
            REQUIRE(data.bytes() != nullptr);

            SUBCASE("release") {
                auto old_bytes = data.bytes();
                auto bytes = data.release();

                REQUIRE(old_bytes == bytes);
                REQUIRE(data.size() == 0);
                REQUIRE(data.bytes() == nullptr);
            }

            SUBCASE("reallocate") {
                data.reallocate(256);

                REQUIRE(data.size() == 256);
                REQUIRE(data.bytes() != nullptr);
            }
        }

        SUBCASE("error handling") {
            shard::dynamic_data data(128);

            SUBCASE("throws") {
                REQUIRE_THROWS_AS(data.reallocate(g_too_much), std::bad_alloc);
            }

            SUBCASE("pointer remains valid after exception") {
                auto old_bytes = data.bytes();

                try {
                    data.reallocate(g_too_much);
                } catch (...) {}

                REQUIRE(old_bytes == data.bytes());
            }
        }
    }
}