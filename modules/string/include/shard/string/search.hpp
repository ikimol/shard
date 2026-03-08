// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#pragma once

#include <string>
#include <vector>

namespace shard {
namespace string {
namespace detail {

inline void make_lps(const std::string& pattern, std::vector<std::size_t>& lps) {
    // length of longest prefix which is also a suffix for the previous index
    std::size_t length = 0;

    lps[0] = 0; // always 0

    std::size_t i = 1;
    while (i < pattern.length()) {
        if (pattern[i] == pattern[length]) {
            length++;
            lps[i] = length;
            i++;
        } else {
            if (length != 0) {
                // update to the previous lps to avoid redundant comparisons
                length = lps[length - 1];
            } else {
                // If no matching prefix found, set lps[i] to 0
                lps[i] = 0;
                i++;
            }
        }
    }
}

} // namespace detail

/// Searches the string for all occurrences of the pattern and returns the
/// starting indices of all the matches.
template <typename OutputIt>
void search(const std::string& string, const std::string& pattern, OutputIt it) {
    auto string_length = string.length();
    auto pattern_length = pattern.length();

    std::vector<std::size_t> lps(pattern_length);
    detail::make_lps(pattern, lps);

    // pointers for traversing the string and pattern
    std::size_t i = 0;
    std::size_t j = 0;

    while (i < string_length) {
        if (string[i] == pattern[j]) {
            i++;
            j++;

            // the entire pattern is matched
            if (j == pattern_length) {
                *it = i - j;

                // use lps of previous index to skip unnecessary comparisons
                j = lps[j - 1];
            }
        } else {
            if (j != 0) {
                // use lps of previous index to skip unnecessary comparisons
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
}

} // namespace string

// bring symbols into parent namespace

using string::search;

} // namespace shard
