// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/log/level.hpp"
#include "shard/log/sink.hpp"

#include <string>
#include <vector>

namespace shard::log {

/// Represents a logger instance with potentially multiple sinks
class logger {
public:
    explicit logger(std::string name, level::type min_level);

    /// Get the name of this logger
    const std::string& name() const { return m_name; }

    /// Get the minimum message level this logger will write
    level::type min_level() const { return m_min_level; }

    /// Add a new sink
    void add_sink(sink_ptr sink);

    /// Remove a previously added sink
    void remove_sink(const sink_ptr& sink);

    /// Send the entry to every added sink
    void write(const entry& entry) const;

private:
    std::string m_name;
    level::type m_min_level;
    std::vector<sink_ptr> m_sinks;
};

} // namespace shard::log
