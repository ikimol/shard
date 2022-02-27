// Copyright (c) 2022 Miklos Molnar. All rights reserved.

#ifndef SHARD_UTILITY_ERROR_HPP
#define SHARD_UTILITY_ERROR_HPP

#include <stdexcept>
#include <string>
#include <utility>

namespace shard {
namespace utility {

template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
class basic_error : public std::runtime_error {
public:
    basic_error(T code, const char* message) : std::runtime_error(message), m_code(code) {}

    basic_error(T code, const std::string& message) : std::runtime_error(message), m_code(code) {}

    T code() const { return m_code; }

    const char* message() const { return what(); }

private:
    T m_code;
};

using error = basic_error<int>;

} // namespace utility

// bring symbols into parent namespace

using utility::error;

} // namespace shard

#endif // SHARD_UTILITY_ERROR_HPP
