// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_STRING_COMPARE_HPP
#define SHARD_STRING_COMPARE_HPP

#include "shard/string/ctype.hpp"

#include <algorithm>
#include <string>

namespace shard {
namespace string {

/// Check if two strings are equal ignoring case
inline bool iequals(const std::string& lhs, const std::string& rhs) {
    if (lhs.length() != rhs.length()) {
        return false;
    }
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), ch::iequal);
}

/// Check whether the string starts with the given prefix
inline bool starts_with(const std::string& s, const std::string& prefix) {
    if (prefix.size() > s.size()) {
        return false;
    }
    return std::equal(prefix.begin(), prefix.end(), s.begin());
}

/// Check whether the string ends with the given suffix
inline bool ends_with(const std::string& s, const std::string& suffix) {
    if (suffix.size() > s.size()) {
        return false;
    }
    return std::equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}

/// Check whether the string contains the given infix
inline bool contains(const std::string& s, const std::string& infix) {
    if (infix.size() > s.size()) {
        return false;
    }
    return s.find(infix) != std::string::npos;
}

} // namespace string

// bring symbols into parent namespace

using string::contains;
using string::ends_with;
using string::iequals;
using string::starts_with;

} // namespace shard

#endif // SHARD_STRING_COMPARE_HPP
