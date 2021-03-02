// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_CASE_CONVERT_HPP
#define SHARD_CASE_CONVERT_HPP

#include "shard/string/ctype.hpp"

#include <algorithm>
#include <string>

namespace shard {
namespace string {

/// Convert the string to all lowercase in-place
inline void to_lower(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(), ch::to_lower);
}

/// Convert the string to all lowercase
inline std::string to_lower_copy(std::string s) {
    to_lower(s);
    return s;
}

/// Convert the string to all uppercase in-place
inline void to_upper(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(), ch::to_upper);
}

/// Convert the string to all uppercase
inline std::string to_upper_copy(std::string str) {
    to_upper(str);
    return str;
}

/// Convert the string to capitalized in-place
inline void to_capitalized(std::string& s) {
    if (!s.empty()) {
        s[0] = ch::to_upper(s[0]);
    }
}

/// Convert the string to capitalized
inline std::string to_capitalized_copy(std::string s) {
    to_capitalized(s);
    return s;
}

/// Convert 'lowerCamelCase' and 'UpperCamelCase' strings to 'snake_case'
inline std::string to_snake_case_copy(std::string s) {
    std::string copy(1, ch::to_lower(s.front()));
    // place underscores between contiguous lower and upper case letters
    for (auto it = s.begin() + 1; it != s.end(); ++it) {
        if (ch::is_upper(*it) && *(it - 1) != '_' && ch::is_lower(*(it - 1))) {
            copy.push_back('_');
        }
        copy.push_back(ch::to_lower(*it));
    }
    return copy;
}

} // namespace string

// bring symbols into parent namespace

using string::to_lower;
using string::to_lower_copy;
using string::to_upper;
using string::to_upper_copy;
using string::to_capitalized;
using string::to_capitalized_copy;

} // namespace shard

#endif // SHARD_CASE_CONVERT_HPP
