// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_STRING_TRIM_HPP
#define SHARD_STRING_TRIM_HPP

#include <algorithm>
#include <cctype>
#include <string>

namespace shard {
namespace string {

/// Left-trim a string in-place, removing all whitespaces from the start
inline void ltrim(std::string& s) {
    auto it = std::find_if(s.begin(), s.end(), [](int ch) { return !::isspace(ch); });
    s.erase(s.begin(), it);
}

/// Left-trim a string, removing all whitespaces from the start
inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

/// Right-trim a string in-place, removing all whitespaces from the end
inline void rtrim(std::string& s) {
    auto it = std::find_if(s.rbegin(), s.rend(), [](int ch) { return !::isspace(ch); });
    s.erase(it.base(), s.end());
}

/// Right-trim a string, removing all whitespaces from the end
inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

/// Trim a string in-place, removing all whitespaces from the start and the end
inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

/// Trim a string, removing all whitespaces from the start and the end
inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

} // namespace string

// bring symbols into parent namespace

using string::ltrim;
using string::ltrim_copy;
using string::rtrim;
using string::rtrim_copy;
using string::trim;
using string::trim_copy;

} // namespace shard

#endif // SHARD_STRING_TRIM_HPP
