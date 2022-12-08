// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <benchpress.hpp>

#include <shard/random.hpp>

#include <random>

BENCHMARK("shard::random", [](benchpress::context* ctx) {
    shard::random<int> r_int(0, 10);
    ctx->reset_timer();
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto n = r_int.next();
        benchpress::escape(&n);
        benchpress::clobber();
    }
})

BENCHMARK("shard::fast_random", [](benchpress::context* ctx) {
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto n = shard::fast_random(0, 10);
        benchpress::escape(&n);
        benchpress::clobber();
    }
})

BENCHMARK("std::mt19937", [](benchpress::context* ctx) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 10);
    ctx->reset_timer();
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto n = dist(mt);
        benchpress::escape(&n);
        benchpress::clobber();
    }
})

BENCHMARK("::rand", [](benchpress::context* ctx) {
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto n = (::rand() % (10 - 0 + 1)) + 0;
        benchpress::escape(&n);
        benchpress::clobber();
    }
})