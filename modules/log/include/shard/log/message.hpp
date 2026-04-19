// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/log/entry.hpp"
#include "shard/log/logger.hpp"

#include <sstream>

namespace shard::log {

/// Used for building the message of a single log entry
class message {
public:
    message(logger& logger, level::type level, std::time_t timestamp, const source_location& location);

    /// Flushes the internal stream
    ~message();

    /// Send a value to the underlying stream
    template <typename T>
    message& operator<<(T&& value) {
        m_stream << std::forward<T>(value);
        return *this;
    }

private:
    // write the message to the logger
    void write();

private:
    logger& m_logger;
    entry m_entry;
    std::ostringstream m_stream;
};

namespace detail {

/// A no-op message used for debug logs in release builds
class null_message {
public:
    template <typename T>
    const null_message& operator<<(T&&) const {
        return *this;
    }
};

} // namespace detail
} // namespace shard::log
