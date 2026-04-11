// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#pragma once

namespace shard {

/// Represents the location of a piece of source code
struct source_location {
    int line;
    const char* file_name;
    const char* function_name;
};

} // namespace shard

#if defined(_MSC_VER)
#define SHARD_PRETTY_FUNCTION __FUNCSIG__
#else
#define SHARD_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

#define SHARD_CURRENT_SOURCE_LOCATION                                                                                  \
    ::shard::source_location {                                                                                         \
        __LINE__, __FILE__, SHARD_PRETTY_FUNCTION                                                                   \
    }
