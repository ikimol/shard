// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include <benchpress.hpp>

#include <shard/dynamic_bitset.hpp>

using shard::dynamic_bitset;

static dynamic_bitset<std::uint64_t> create_test_bitset(std::size_t size, std::size_t set_every_n = 3) {
    dynamic_bitset<std::uint64_t> bitset(size);
    for (std::size_t i = 0; i < size; i += set_every_n) {
        bitset.set(i);
    }
    return bitset;
}

BENCHMARK("dynamic_bitset::contains", [](benchpress::context* ctx) {
    auto lhs = create_test_bitset(64, 3);
    auto rhs = create_test_bitset(64, 6);

    ctx->reset_timer();
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto result = lhs.contains(rhs);
        benchpress::escape(&result);
        benchpress::clobber();
    }
})

BENCHMARK("dynamic_bitset::operator& + compare", [](benchpress::context* ctx) {
    auto lhs = create_test_bitset(64, 3);
    auto rhs = create_test_bitset(64, 6);

    ctx->reset_timer();
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto result = (lhs & rhs) == rhs;
        benchpress::escape(&result);
        benchpress::clobber();
    }
})

BENCHMARK("dynamic_bitset::find_first", [](benchpress::context* ctx) {
    auto bitset = create_test_bitset(128, 32);

    ctx->reset_timer();

    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto result = bitset.find_first();
        benchpress::escape(&result);
        benchpress::clobber();
    }
})

BENCHMARK("dynamic_bitset::find_next", [](benchpress::context* ctx) {
    auto bitset = create_test_bitset(128, 32);

    ctx->reset_timer();

    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto result = bitset.find_next(i % 128);
        benchpress::escape(&result);
        benchpress::clobber();
    }
})

BENCHMARK("dynamic_bitset::iteration", [](benchpress::context* ctx) {
    auto bitset = create_test_bitset(128, 4);

    ctx->reset_timer();
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::size_t count = 0;
        for (auto idx = bitset.find_first(); idx != dynamic_bitset<std::uint64_t>::npos; idx = bitset.find_next(idx)) {
            ++count;
        }
        benchpress::escape(&count);
        benchpress::clobber();
    }
})

BENCHMARK("dynamic_bitset::count", [](benchpress::context* ctx) {
    auto bitset = create_test_bitset(128, 3);

    ctx->reset_timer();
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto result = bitset.count();
        benchpress::escape(&result);
        benchpress::clobber();
    }
})

BENCHMARK("dynamic_bitset::operator&=", [](benchpress::context* ctx) {
    auto bitset1 = create_test_bitset(128, 3);
    auto bitset2 = create_test_bitset(128, 5);

    ctx->reset_timer();
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        bitset1 &= bitset2;
        benchpress::clobber();
    }
})

BENCHMARK("dynamic_bitset::operator|=", [](benchpress::context* ctx) {
    auto bitset1 = create_test_bitset(128, 3);
    auto bitset2 = create_test_bitset(128, 5);

    ctx->reset_timer();
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        bitset1 |= bitset2;
        benchpress::clobber();
    }
})
