// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/signal.hpp>

#include <doctest.h>

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

TEST_SUITE("signal") {
    TEST_CASE("slots") {
        shard::signal<int> event;

        SUBCASE("free function") {
            event.connect(on_event);
            event.connect(on_empty_event);
            event.emit(42);
            CHECK(g_last_i == 42);
            CHECK(g_handled);
        }

        SUBCASE("lambda") {
            int last_i = 0;
            event.connect([&last_i](int i) { last_i = i; });
            event.emit(42);
            CHECK(last_i == 42);
        }

        SUBCASE("member function") {
            event_handler handler;
            event.connect(&event_handler::on_event, &handler);
            event.connect(&event_handler::on_empty_event, &handler);
            event.emit(42);
            CHECK(handler.last_i == 42);
            CHECK(handler.handled);
        }
    }

    TEST_CASE("connections") {
        shard::signal<int> event;

        SUBCASE("disable") {
            int last_i = 0;
            auto c = event.connect([&](int i) { last_i = i; });
            CHECK(event.slot_count() == 1);

            c.disable();
            CHECK(event.slot_count() == 1);

            event.emit(42);
            CHECK(last_i == 0);

            c.enable();
            CHECK(event.slot_count() == 1);

            event.emit(21);
            CHECK(last_i == 21);
        }

        SUBCASE("disconnect") {
            int last_i = 0;
            auto c = event.connect([&](int i) { last_i = i; });
            CHECK(event.slot_count() == 1);

            c.disconnect();
            CHECK(event.slot_count() == 0);

            event.emit(42);
            CHECK(last_i == 0);
        }

        SUBCASE("scoped") {
            int last_i = 0;

            CHECK(event.slot_count() == 0);
            {
                shard::scoped_connection c = event.connect([&](int i) { last_i = i; });
                CHECK(event.slot_count() == 1);

                event.emit(42);
                CHECK(last_i == 42);
            }
            CHECK(event.slot_count() == 0);

            event.emit(21);
            CHECK(last_i == 42);
        }
    }
}