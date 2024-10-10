// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/signal.hpp>

#include <doctest.h>

#include <utility>

static int g_last_i = 0;
static bool g_handled = false;

static void on_event(int i) {
    g_last_i = i;
}

static void on_empty_event() {
    g_handled = true;
}

struct event_handler {
    void on_event(int i) { last_i = i; }

    void on_empty_event() { handled = true; }

    int last_i = 0;
    bool handled = false;
};

TEST_CASE("signal") {
    SUBCASE("slots") {
        shard::signal<int> event;

        SUBCASE("free function") {
            auto c1 = event.connect(on_event);
            auto c2 = event.connect(on_empty_event);
            event.emit(42);
            REQUIRE(g_last_i == 42);
            REQUIRE(g_handled);
        }

        SUBCASE("lambda") {
            int last_i = 0;
            auto c = event.connect([&last_i](int i) { last_i = i; });
            event.emit(42);
            REQUIRE(last_i == 42);
        }

        SUBCASE("member function") {
            event_handler handler;
            auto c1 = event.connect(shard::bind(&handler, &event_handler::on_event));
            auto c2 = event.connect(shard::bind(&handler, &event_handler::on_empty_event));
            event.emit(42);
            REQUIRE(handler.last_i == 42);
            REQUIRE(handler.handled);
        }
    }

    SUBCASE("connections") {
        shard::signal<int> event;

        SUBCASE("set_enabled") {
            int last_i = 0;
            auto c = event.connect([&](int i) { last_i = i; });
            REQUIRE(event.slot_count() == 1);

            c.set_enabled(false);
            REQUIRE(event.slot_count() == 1);

            event.emit(42);
            REQUIRE(last_i == 0);

            c.set_enabled(true);
            REQUIRE(event.slot_count() == 1);

            event.emit(21);
            REQUIRE(last_i == 21);
        }

        SUBCASE("disconnect") {
            int last_i = 0;
            auto c = event.connect([&](int i) { last_i = i; });
            REQUIRE(event.slot_count() == 1);

            c.disconnect();
            REQUIRE(event.slot_count() == 0);

            event.emit(42);
            REQUIRE(last_i == 0);
        }

        SUBCASE("scoping") {
            int last_i = 0;

            REQUIRE(event.slot_count() == 0);
            {
                auto c = event.connect([&](int i) { last_i = i; });
                REQUIRE(event.slot_count() == 1);

                event.emit(42);
                REQUIRE(last_i == 42);
            }
            REQUIRE(event.slot_count() == 0);

            event.emit(21);
            REQUIRE(last_i == 42);
        }
    }
}