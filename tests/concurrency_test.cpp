// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include "helpers/no_default.hpp"
#include "helpers/widget.hpp"

#include <shard/concurrency.hpp>

#include <catch.hpp>

#include <thread>

static void channel_writer(shard::channel<int>* channel, int i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    channel->push(i);
}

TEST_CASE("concurrency") {
    SECTION("channel") {
        shard::channel<int> channel;

        SECTION("try_pop") {
            channel.push(42);
            int n = -1;
            REQUIRE(channel.try_pop(n));
            REQUIRE(n == 42);
        }

        SECTION("pop") {
            std::thread thread(channel_writer, &channel, 42);
            int n = -1;
            auto start = std::chrono::high_resolution_clock::now();
            auto success = channel.pop(n);
            auto duration = std::chrono::high_resolution_clock::now() - start;
            REQUIRE(success);
            REQUIRE(n == 42);
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            // +-50 milliseconds tolerance
            REQUIRE(ms > 50);
            REQUIRE(ms < 150);
            thread.join();
        }

        SECTION("storing class with no default constructor") {
            shard::channel<test::no_default> no_default_channel;
            no_default_channel.emplace(42, 21);

            auto n = no_default_channel.try_pop();
            REQUIRE(n);
            REQUIRE(n->a == 42);
            REQUIRE(n->b == 21);
        }
    }

    SECTION("thread_safe") {
        using thread_safe_widget = shard::rw_thread_safe<test::widget>;
        thread_safe_widget widget(42, 21);

        SECTION("reading") {
            shard::read_access<thread_safe_widget> w(widget);
            REQUIRE(w->a == 42);
            REQUIRE(w->b == 21);
        }

        SECTION("writing") {
            {
                shard::write_access<thread_safe_widget> w(widget);
                std::swap(w->a, w->b);
            }

            {
                shard::read_access<thread_safe_widget> w(widget);
                REQUIRE(w->a == 21);
                REQUIRE(w->b == 42);
            }
        }
    }
}
