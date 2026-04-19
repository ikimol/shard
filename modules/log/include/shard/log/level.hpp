// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/enums/traits.hpp>

namespace shard::log::level {

/// The level of a log entry
enum type : std::uint8_t { trace, debug, info, warn, error, fatal };

} // namespace shard::log::level

template <>
struct shard::enum_traits<shard::log::level::type> {
    static constexpr std::array values = {
        log::level::trace,
        log::level::debug,
        log::level::info,
        log::level::warn,
        log::level::error,
        log::level::fatal,
    };

    static constexpr std::array names = {
        "trace",
        "debug",
        "info",
        "warn",
        "error",
        "fatal",
    };

    static constexpr std::array chars = {'T', 'D', 'I', 'W', 'E', 'F'};
};
