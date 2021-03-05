// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_STRING_TO_STRING_HPP
#define SHARD_STRING_TO_STRING_HPP

#include <shard/meta/type_traits.hpp>

#include <sstream>
#include <string>
#include <utility>

namespace shard {
namespace string {
namespace detail {

inline std::string to_string(std::nullptr_t) {
    return "(null)";
}

inline std::string to_string(char value) {
    return std::string(1, value);
}

inline std::string to_string(bool value) {
    return value ? "true" : "false";
}

inline std::string to_string(const char* value) {
    return value;
}

inline std::string to_string(std::string value) {
    return value;
}

/// Convert an arbitrary value to a string using a stream
template <typename T, typename = std::enable_if_t<meta::is_streamable<std::ostream, T>::value>>
std::string to_string(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

} // namespace detail

/// Convert an arbitrary type to a string
template <typename T>
std::string to_string(T&& value) {
    using detail::to_string;
    using std::to_string;
    return to_string(std::forward<T>(value));
}

} // namespace string

// bring symbols into parent namespace

using string::to_string;

} // namespace shard

#endif // SHARD_STRING_TO_STRING_HPP
