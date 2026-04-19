// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#include "shard/log/message.hpp"

namespace shard::log {

message::message(logger& logger, level::type level, std::time_t timestamp, const source_location& location)
: m_logger(logger) {
    m_entry.level = level;
    m_entry.timestamp = timestamp;
    m_entry.location = location;
}

message::~message() {
    write();
}

void message::write() {
    m_entry.message = m_stream.str();
    m_logger.write(m_entry);
}

} // namespace shard::log
