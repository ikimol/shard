// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include <benchpress.hpp>

#include <shard/signal.hpp>

namespace {

std::size_t g_invoke_count = 64;
std::size_t g_counter = 0;

void callback(void* ptr, std::size_t value) {
    g_counter += value;
    benchpress::escape(&ptr);
    benchpress::escape(&value);
    benchpress::escape(&g_counter);
}

void (*g_callback_ptr)(void*, std::size_t) = callback;

} // namespace

using signal_type = shard::signal<void*, std::size_t>;

BENCHMARK("shard::signal", [](benchpress::context* ctx) {
    signal_type event;
    std::vector<shard::connection> connections;
    for (std::size_t i = 0; i < g_invoke_count; ++i) {
        connections.push_back(event.connect(callback));
    }
    ctx->reset_timer();
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        event.emit(nullptr, i);
        benchpress::clobber();
    }
})

BENCHMARK("function pointer", [](benchpress::context* ctx) {
    void (*function_ptr)(void*, std::size_t) = g_callback_ptr;
    ctx->reset_timer();
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        for (std::size_t j = 0; j < g_invoke_count; ++j) {
            function_ptr(nullptr, i);
        }
        benchpress::clobber();
    }
})
