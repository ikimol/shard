// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <cstdarg>
#include <cstddef>
#include <string>
#include <string_view>

#if defined(__GNUC__)
#define SHARD_PRINTF_STYLE(n) __attribute__((format(__printf__, n, n + 1)))
#else
#define SHARD_PRINTF_STYLE(n)
#endif

namespace shard {
namespace string {

/// Create a string inside the buffer using a format and a number of arguments
std::string_view sfmt(char* buffer, std::size_t size, const char* format, ...) SHARD_PRINTF_STYLE(3);

/// Create a string using a format and a number of arguments
std::string fmt(const char* format, ...) SHARD_PRINTF_STYLE(1);

/// Create a string using a format and a number of arguments
std::string fmt(const char* format, va_list ap);

} // namespace string

// bring symbols into parent namespace

using string::fmt;
using string::sfmt;

} // namespace shard

#define SHARD_MAKE_FMT(length, format)                                                                                 \
    [](auto&&... args) {                                                                                               \
        static char buffer[length];                                                                                    \
        std::snprintf(buffer, sizeof(buffer), (format), std::forward<decltype(args)>(args)...);                        \
        return std::string(buffer);                                                                                    \
    }

#undef SHARD_PRINTF_STYLE

#include "shard/string/fmt.inl"
