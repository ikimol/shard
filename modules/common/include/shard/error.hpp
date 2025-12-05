// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <stdexcept>
#include <string>
#include <utility>

namespace shard {

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
class basic_error : public std::runtime_error {
public:
    basic_error(T code, const char* message)
    : std::runtime_error(message)
    , m_code(code) {}

    basic_error(T code, const std::string& message)
    : std::runtime_error(message)
    , m_code(code) {}

    T code() const { return m_code; }

    const char* message() const { return what(); }

private:
    T m_code;
};

using error = basic_error<int>;

} // namespace shard
