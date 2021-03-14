// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/signal.hpp>

#include <catch.hpp>

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
    SECTION("slots") {
        shard::signal<int> event;

        SECTION("free function") {
            event.connect(on_event);
            event.connect(on_empty_event);
            event.emit(42);
            REQUIRE(g_last_i == 42);
            REQUIRE(g_handled);
        }

        SECTION("lambda") {
            int last_i = 0;
            event.connect([&last_i](int i) { last_i = i; });
            event.emit(42);
            REQUIRE(last_i == 42);
        }

        SECTION("member function") {
            event_handler handler;
            event.connect(&event_handler::on_event, &handler);
            event.connect(&event_handler::on_empty_event, &handler);
            event.emit(42);
            REQUIRE(handler.last_i == 42);
            REQUIRE(handler.handled);
        }
    }

    SECTION("connections") {
        shard::signal<int> event;

        SECTION("disable") {
            int last_i = 0;
            auto c = event.connect([&](int i) { last_i = i; });
            REQUIRE(event.slot_count() == 1);

            c.disable();
            REQUIRE(event.slot_count() == 1);

            event.emit(42);
            REQUIRE(last_i == 0);

            c.enable();
            REQUIRE(event.slot_count() == 1);

            event.emit(21);
            REQUIRE(last_i == 21);
        }

        SECTION("disconnect") {
            int last_i = 0;
            auto c = event.connect([&](int i) { last_i = i; });
            REQUIRE(event.slot_count() == 1);

            c.disconnect();
            REQUIRE(event.slot_count() == 0);

            event.emit(42);
            REQUIRE(last_i == 0);
        }

        SECTION("scoped") {
            int last_i = 0;

            REQUIRE(event.slot_count() == 0);
            {
                shard::scoped_connection c = event.connect([&](int i) { last_i = i; });
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