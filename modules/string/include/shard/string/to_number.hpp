// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_STRING_TO_NUMBER_HPP
#define SHARD_STRING_TO_NUMBER_HPP

#include "shard/string/trim.hpp"

#include <shard/optional.hpp>

#include <string>

namespace shard {
namespace string {

inline shard::optional<int> to_int(const std::string& str) {
    try {
        std::size_t idx;
        auto value = std::stoi(str, &idx, 10);
        if (idx == str.length()) {
            return value;
        }
    } catch (...) {
        // not an integer
    }
    return shard::nullopt;
}

inline shard::optional<unsigned long> to_unsigned(const std::string& str) {
    auto str_trimmed = ltrim_copy(str);
    if (!str_trimmed.empty() && str_trimmed[0] == '-') {
        return shard::nullopt;
    }

    try {
        std::size_t idx;
        auto value = std::stoul(str, &idx, 10);
        if (idx == str.length()) {
            return value;
        }
    } catch (...) {
        // not an unsigned
    }
    return shard::nullopt;
}

inline shard::optional<float> to_float(const std::string& str) {
    try {
        std::size_t idx;
        auto value = std::stof(str, &idx);
        if (idx == str.length()) {
            return value;
        }
    } catch (...) {
        // not a float
    }
    return shard::nullopt;
}

inline shard::optional<double> to_double(const std::string& str) {
    try {
        std::size_t idx;
        auto value = std::stod(str, &idx);
        if (idx == str.length()) {
            return value;
        }
    } catch (...) {
        // not a double
    }
    return shard::nullopt;
}

} // namespace string

// bring symbols into parent namespace

using string::to_double;
using string::to_float;
using string::to_int;
using string::to_unsigned;

} // namespace shard

#endif // SHARD_STRING_TO_NUMBER_HPP
