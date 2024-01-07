// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include "helpers/no_default.hpp"
#include "helpers/widget.hpp"

#include <shard/concurrency.hpp>

#include <doctest.h>

#include <thread>

static void channel_writer(shard::channel<int>* channel, int i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    channel->push(i);
}

TEST_CASE("concurrency") {
    SUBCASE("channel") {
        shard::channel<int> channel;

        SUBCASE("try_pop") {
            channel.push(42);
            int n = -1;
            REQUIRE(channel.try_pop(n));
            REQUIRE(n == 42);
        }

        SUBCASE("pop") {
            std::thread thread(channel_writer, &channel, 42);
            int n = -1;
            auto start = std::chrono::high_resolution_clock::now();
            auto success = channel.pop(n);
            auto duration = std::chrono::high_resolution_clock::now() - start;
            REQUIRE(success);
            REQUIRE(n == 42);
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            // 90 milliseconds tolerance
            REQUIRE(ms < 100);
            thread.join();
        }

        SUBCASE("storing class with no default constructor") {
            shard::channel<test::no_default> no_default_channel;
            no_default_channel.emplace(42, 21);

            auto n = no_default_channel.try_pop();
            REQUIRE(n);
            REQUIRE(n->a == 42);
            REQUIRE(n->b == 21);
        }
    }

    SUBCASE("mpsc_queue") {
        shard::mpsc_queue<test::widget, 8> queue;

        SUBCASE("size") {
            REQUIRE(queue.is_empty());
            REQUIRE(queue.size() == 0);
            REQUIRE(queue.capacity() == 8);
        }

        SUBCASE("push") {
            queue.push(test::widget {1});
            queue.push(test::widget {2});
            REQUIRE(queue.size() == 2);
        }

        SUBCASE("emplace") {
            queue.emplace(1);
            queue.emplace(2);
            REQUIRE(queue.size() == 2);
        }

        SUBCASE("pop") {
            queue.emplace(1);
            queue.emplace(2);
            queue.emplace(3);
            REQUIRE(queue.size() == 3);
            int expected[] = {1, 2, 3};
            for (auto i : expected) {
                auto value = queue.pop();
                REQUIRE(value.has_value());
                REQUIRE(value->a == i);
            }
            REQUIRE(queue.is_empty());
        }
    }

    SUBCASE("thread_safe") {
        using thread_safe_widget = shard::rw_thread_safe<test::widget>;
        thread_safe_widget widget(42, 21);

        SUBCASE("reading") {
            shard::read_access<thread_safe_widget> w(widget);
            REQUIRE(w->a == 42);
            REQUIRE(w->b == 21);
        }

        SUBCASE("writing") {
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
