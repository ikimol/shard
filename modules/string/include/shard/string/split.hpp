// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_STRING_SPLIT_HPP
#define SHARD_STRING_SPLIT_HPP

#include <sstream>
#include <string>

namespace shard {
namespace string {

/// Split a string by the delimiter and using the iterator
///
/// \note The iterator is *NOT* incremented by this function so an appropriate
///       iterator must be used (e.g. std::back_insert_iterator).
template <typename OutputIt>
void split(const std::string& s, OutputIt out, char delim) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *out = item;
    }
}

/// Split a string by the delimiter functor and using the iterator
///
/// \note The iterator is *NOT* incremented by this function so an appropriate
///       iterator must be used (e.g. std::back_insert_iterator).
template <typename OutputIt, typename UnaryPredicate>
void split(const std::string& s, OutputIt out, UnaryPredicate p) {
    auto pos = 0;
    for (auto i = 0ul; i < s.length(); ++i) {
        if (p(s[i])) {
            *out = s.substr(pos, (i - pos));
            pos = i + 1;
        }
    }
    *out = s.substr(pos);
}

} // namespace string

// bring symbols into parent namespace

using string::split;

} // namespace shard

#endif // SHARD_STRING_SPLIT_HPP
