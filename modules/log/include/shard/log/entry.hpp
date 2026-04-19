// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/log/level.hpp"

#include <shard/source_location.hpp>

#include <ctime>
#include <string>

namespace shard::log {

/// Represents a single log entry
struct entry {
    level::type level;
    std::time_t timestamp;
    source_location location;
    std::string message;
};

} // namespace shard::log
