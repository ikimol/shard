// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#include "shard/log/logger.hpp"

#include <algorithm>
#include <utility>

namespace shard::log {

logger::logger(std::string name, level::type min_level)
: m_name(std::move(name))
, m_min_level(min_level) {}

void logger::add_sink(sink_ptr sink) {
    m_sinks.push_back(std::move(sink));
}

void logger::remove_sink(const sink_ptr& sink) {
    m_sinks.erase(std::remove(m_sinks.begin(), m_sinks.end(), sink), m_sinks.end());
}

void logger::write(const entry& entry) const {
    for (const auto& sink : m_sinks) {
        sink->write(*this, entry);
    }
}

} // namespace shard::log
