// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <cassert>
#include <cstdio>

namespace shard::string {

inline std::string_view sfmt(char* buffer, std::size_t size, const char* format, ...) {
    assert(size > 0);
    std::va_list args;
    va_start(args, format);
    // create the string in the buffer
    std::vsnprintf(buffer, size, format, args);
    va_end(args);
    return buffer;
}

inline std::string fmt(const char* format, ...) {
    // calculate the number of characters that would be written
    std::va_list args1;
    va_start(args1, format);
    std::va_list args2;
    va_copy(args2, args1);
    const auto size = std::vsnprintf(nullptr, 0, format, args1);
    va_end(args1);
    assert(size > 0);
    // create a ch buffer with the correct size (+1 for null terminator)
    auto buffer = new char[size + 1];
    std::vsnprintf(buffer, size + 1, format, args2);
    va_end(args2);
    // construct the string and destroy the buffer
    std::string result(buffer, buffer + size);
    delete[] buffer;
    return result;
}

inline std::string vfmt(const char* format, va_list ap) {
    std::va_list args_copy;
    va_copy(args_copy, ap);
    const auto size = std::vsnprintf(nullptr, 0, format, args_copy);
    va_end(args_copy);
    assert(size > 0);
    // create a ch buffer with the correct size (+1 for null terminator)
    auto buffer = new char[size + 1];
    std::vsnprintf(buffer, size + 1, format, ap);
    // construct the string and destroy the buffer
    std::string result(buffer, buffer + size);
    delete[] buffer;
    return result;
}

} // namespace shard::string
