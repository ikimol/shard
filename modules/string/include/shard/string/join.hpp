// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_STRING_JOIN_HPP
#define SHARD_STRING_JOIN_HPP

#include "shard/string/to_string.hpp"

#include <iterator>
#include <string>

namespace shard {
namespace string {

/// Create a string from a sequence of values and a delimiter
template <typename Range>
std::string join(const Range& r, const std::string& delimiter) {
    std::string result;
    auto it = std::begin(r);
    auto end = std::end(r);
    // append first element
    if (it != end) {
        result.append(to_string(*it));
        ++it;
    }
    // append rest of the elements
    for (; it != end; ++it) {
        // add separator
        result.append(delimiter);
        // add element
        result.append(to_string(*it));
    }
    return result;
}

} // namespace string

// bring symbols into parent namespace

using string::join;

} // namespace shard

#endif // SHARD_STRING_JOIN_HPP
