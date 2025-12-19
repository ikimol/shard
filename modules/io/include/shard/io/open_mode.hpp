// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/enums/flags.hpp>

namespace shard::io {

enum class open_mode : unsigned char {
    read = 1 << 0,      ///< Open for reading
    write = 1 << 1,     ///< Open for writing, truncate contents, create if does not exist
    append = 1 << 2,    ///< Open for writing, append to end, create if does not exist
    binary = 1 << 3,    ///< Open in binary mode
    extended = 1 << 4,  ///< Open the file for reading / writing
    exclusive = 1 << 5, ///< Fail if the file already exists
};

SHARD_DECLARE_FLAGS(open_mode_t, open_mode);

/// Convert the open mode to its string representation
inline std::string to_string(open_mode_t mode) {
    std::string result;
    result.reserve(4);

    if (mode.test(open_mode::read)) {
        result.push_back('r');
    } else if (mode.test(open_mode::write)) {
        result.push_back('w');
    } else if (mode.test(open_mode::append)) {
        result.push_back('a');
    }

    if (mode.test(open_mode::binary)) {
        result.push_back('b');
    }
    if (mode.test(open_mode::extended)) {
        result.push_back('+');
    }
    if (mode.test(open_mode::exclusive)) {
        result.push_back('x');
    }

    return result;
}

} // namespace shard::io

SHARD_DECLARE_FLAG_OPERATORS(shard::io::open_mode_t)
