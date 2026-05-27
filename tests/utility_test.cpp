// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/utility/defer.hpp>
#include <shard/utility/exception_guard.hpp>
#include <shard/utility/function_ref.hpp>
#include <shard/utility/lazy.hpp>
#include <shard/utility/span.hpp>

#include <stdexcept>
#include <string>
#include <vector>

#include <doctest.h>

static int add(int a, int b) {
    return a + b;
}

TEST_CASE("utility") {
    SUBCASE("defer") {
        bool called = false;
        {
            auto _ = shard::defer([&] { called = true; });
        }
        REQUIRE(called);
    }

    SUBCASE("exception_guard") {
        bool did_rollback = false;

        SUBCASE("throw") {
            {
                auto guard = shard::make_exception_guard([&] { did_rollback = true; });
                try {
                    throw std::runtime_error("test error");
                } catch (std::exception& e) {}
            }
            REQUIRE(did_rollback);
        }

        SUBCASE("nothrow") {
            {
                auto guard = shard::make_exception_guard([&] { did_rollback = true; });
                try {
                    guard.dismiss();
                } catch (std::exception& e) {}
            }
            REQUIRE_FALSE(did_rollback);
        }
    }

    SUBCASE("function_ref") {
        SUBCASE("from lambda") {
            int x = 0;
            auto lambda = [&] { ++x; };
            shard::function_ref<void()> f(lambda);
            f();
            REQUIRE(x == 1);
        }

        SUBCASE("from function pointer") {
            shard::function_ref f(&add);
            REQUIRE(f(3, 4) == 7);
        }

        SUBCASE("from functor") {
            struct multiply {
                int factor;

                int operator()(int x) const { return x * factor; }
            };

            multiply m {5};
            shard::function_ref<int(int)> f(m);
            REQUIRE(f(3) == 15);
        }

        SUBCASE("copy constructor") {
            int x = 0;
            auto lambda = [&] { ++x; };
            shard::function_ref<void()> f(lambda);
            shard::function_ref g(f);
            g();
            REQUIRE(x == 1);
        }

        SUBCASE("copy assignment") {
            int a = 0, b = 0;
            auto la = [&] { ++a; };
            auto lb = [&] { ++b; };
            shard::function_ref<void()> f(la);
            shard::function_ref<void()> g(lb);
            f = g;
            f();
            REQUIRE(a == 0);
            REQUIRE(b == 1);
        }

        SUBCASE("assign from callable") {
            int a = 0;
            int b = 0;
            auto la = [&] { ++a; };
            auto lb = [&] { ++b; };
            shard::function_ref<void()> f(la);
            f();
            f = lb;
            f();
            REQUIRE(a == 1);
            REQUIRE(b == 1);
        }

        SUBCASE("swap") {
            int a = 0;
            int b = 0;
            auto la = [&] { ++a; };
            auto lb = [&] { ++b; };
            shard::function_ref<void()> f(la);
            shard::function_ref<void()> g(lb);
            f.swap(g);
            f();
            REQUIRE(a == 0);
            REQUIRE(b == 1);
        }

        SUBCASE("free swap") {
            int a = 0;
            int b = 0;
            auto la = [&] { ++a; };
            auto lb = [&] { ++b; };
            shard::function_ref<void()> f(la);
            shard::function_ref<void()> g(lb);
            swap(f, g);
            f();
            REQUIRE(a == 0);
            REQUIRE(b == 1);
        }
    }

    SUBCASE("lazy") {
        SUBCASE("called once") {
            constexpr int r_value = 42;

            int call_count = 0;

            shard::lazy<int> value([&] {
                ++call_count;
                return r_value;
            });

            REQUIRE(*value == r_value);
            REQUIRE(*value == r_value);
            REQUIRE(*value == r_value);
            REQUIRE(call_count == 1);
        }

        SUBCASE("value initialized") {
            shard::lazy<int> value = 3;
            REQUIRE(*value == 3);
        }
    }

    SUBCASE("span") {
        SUBCASE("from iterator and size") {
            std::vector v = {1, 2, 3};
            shard::span s(v.begin(), v.size());
            REQUIRE(s.size() == 3);
            REQUIRE(s.data() == v.data());
        }

        SUBCASE("from iterator pair") {
            std::vector v = {1, 2, 3};
            shard::span s(v.begin(), v.end());
            REQUIRE(s.size() == 3);
            REQUIRE(s.data() == v.data());
        }

        SUBCASE("from raw array") {
            int a[4] = {1, 2, 3, 4};
            shard::span s(a);
            REQUIRE(s.size() == 4);
            REQUIRE(s.data() == a);
        }

        SUBCASE("from std::array") {
            std::array a = {7, 8, 9};
            shard::span s(a);
            REQUIRE(s.size() == 3);
            REQUIRE(s.data() == a.data());
        }

        SUBCASE("from const std::array") {
            const std::array a = {1, 2, 3};
            shard::span s(a);
            REQUIRE(s.size() == 3);
            REQUIRE(s[0] == 1);
        }

        SUBCASE("from range") {
            std::vector v = {4, 5, 6};
            shard::span s(v);
            REQUIRE(s.size() == 3);
            REQUIRE(s.data() == v.data());
        }

        SUBCASE("element access") {
            int a[3] = {10, 20, 30};
            shard::span s(a);
            REQUIRE(s[0] == 10);
            REQUIRE(s[1] == 20);
            REQUIRE(s[2] == 30);

            REQUIRE(s.at(0) == 10);
            REQUIRE(s.at(1) == 20);
            REQUIRE_THROWS_AS(s.at(3), std::out_of_range);

            REQUIRE(s.front() == 10);
            REQUIRE(s.back() == 30);
        }

        SUBCASE("mutation") {
            int a[3] = {1, 2, 3};
            shard::span s(a);
            s[1] = 42;
            REQUIRE(a[1] == 42);
        }

        SUBCASE("iteration") {
            int a[4] = {1, 2, 3, 4};
            shard::span s(a);
            int sum = 0;
            for (int x : s) {
                sum += x;
            }
            REQUIRE(sum == 10);
        }

        SUBCASE("empty") {
            std::vector v = {1, 2, 3};
            shard::span non_empty(v);
            REQUIRE_FALSE(non_empty.empty());

            shard::span empty_s(v.begin(), v.begin());
            REQUIRE(empty_s.empty());
            REQUIRE(empty_s.size() == 0);
        }

        SUBCASE("first and last") {
            int a[5] = {1, 2, 3, 4, 5};
            shard::span s(a);

            auto f = s.first(3);
            REQUIRE(f.size() == 3);
            REQUIRE(f[0] == 1);
            REQUIRE(f[2] == 3);

            auto l = s.last(2);
            REQUIRE(l.size() == 2);
            REQUIRE(l[0] == 4);
            REQUIRE(l[1] == 5);
        }

        SUBCASE("subspan") {
            int a[5] = {1, 2, 3, 4, 5};
            shard::span s(a);

            auto mid = s.subspan(1, 3);
            REQUIRE(mid.size() == 3);
            REQUIRE(mid[0] == 2);
            REQUIRE(mid[2] == 4);

            auto tail = s.subspan(3);
            REQUIRE(tail.size() == 2);
            REQUIRE(tail[0] == 4);
            REQUIRE(tail[1] == 5);
        }
    }
}
