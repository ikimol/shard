// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <benchpress.hpp>

#include <shard/string/fmt.hpp>

#include <sstream>

BENCHMARK("shard::sfmt", [](benchpress::context* ctx) {
    char buffer[64];
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto s = shard::sfmt(buffer, 64, "message: %d %f %s", 42, 3.141592f, "foo");
        benchpress::escape(&s);
        benchpress::clobber();
    }
})

BENCHMARK("shard::fmt", [](benchpress::context* ctx) {
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        auto s = shard::fmt("message: %d %f %s", 42, 3.141592f, "foo");
        benchpress::escape(&s);
        benchpress::clobber();
    }
})

BENCHMARK("snprintf", [](benchpress::context* ctx) {
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        char buffer[64];
        auto s = ::snprintf(buffer, 64, "message: %d %f %s", 42, 3.141592f, "foo");
        benchpress::escape(&s);
        benchpress::clobber();
    }
});

BENCHMARK("std::sstream", [](benchpress::context* ctx) {
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::ostringstream oss;
        oss << "message: " << 42 << ' ' << 3.141592f << " foo";
        auto s = oss.str();
        benchpress::escape(&s);
        benchpress::clobber();
    }
})

BENCHMARK("std::to_string", [](benchpress::context* ctx) {
    for (std::size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::string s("message: ");
        s += std::to_string(42);
        s += ' ';
        s += std::to_string(3.141592f);
        s += " foo";
        benchpress::escape(&s);
        benchpress::clobber();
    }
})
