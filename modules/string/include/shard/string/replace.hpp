// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_STRING_REPLACE_HPP
#define SHARD_STRING_REPLACE_HPP

#include <string>

namespace shard {
namespace string {

/// Replace all occurrences of a string with another in-place
inline void replace_all(std::string& s, const std::string& from, const std::string& to) {
    auto length = from.length();
    auto pos = s.find(from);
    while (pos != std::string::npos) {
        s.replace(pos, length, to);
        pos = s.find(from, pos + to.length());
    }
}

/// Replace all occurrences of a string with another
inline std::string replace_all_copy(std::string s, const std::string& from, const std::string& to) {
    replace_all(s, from, to);
    return s;
}

} // namespace string

// bring symbols into parent namespace

using string::replace_all;
using string::replace_all_copy;

} // namespace shard

#endif // SHARD_STRING_REPLACE_HPP
