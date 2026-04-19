// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/log/entry.hpp"

#include <memory>
#include <string_view>

namespace shard::log {

class logger;

/// Represents the output function of a sink
using sink_writer = void (*)(const std::string&, const entry&, void*);

/// Represents the cleanup function of a sink
using sink_cleanup = void (*)(void*);

class sink {
public:
    sink(std::string name, sink_writer writer, void* userdata = nullptr, sink_cleanup cleanup = nullptr);

    /// Cleans up aby sink resources
    ~sink();

    /// Get the name of this sink
    const std::string& name() const { return m_name; }

    /// Write the entry
    void write(const logger& logger, const entry& entry) const;

private:
    std::string m_name;
    sink_writer m_writer;
    void* m_userdata;
    sink_cleanup m_cleanup;
};

/// Convenience type for sink pointers
using sink_ptr = std::shared_ptr<sink>;

// helpers

/// A sink that prints the message to the console
///
/// \returns The shared sink instance that writes to the console.
sink_ptr console_sink();

/// A sink that prints the message to the file
sink_ptr make_file_sink(std::string_view path);

} // namespace shard::log
