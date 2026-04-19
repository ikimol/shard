// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#include <shard/log.hpp>
#include <shard/string/fmt.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

static shard::log::logger g_engine_logger = [] {
    auto logger = shard::log::logger("Engine", shard::log::level::trace);
    logger.add_sink(shard::log::console_sink());
    return logger;
}();

static shard::log::logger g_lua_logger = [] {
    auto logger = shard::log::logger("Lua", shard::log::level::trace);
    logger.add_sink(shard::log::console_sink());
    if (auto file_sink = shard::log::make_file_sink("log.txt")) {
        logger.add_sink(file_sink);
    }
    return logger;
}();

int main(int /* argc */, char* /* argv */[]) {
    SHARD_DLOG(g_engine_logger) << 42;
    SHARD_ELOG(g_engine_logger) << "whoops";
    SHARD_ILOG(g_lua_logger) << "hello";
}
