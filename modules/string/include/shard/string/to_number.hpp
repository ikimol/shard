// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_STRING_TO_NUMBER_HPP
#define SHARD_STRING_TO_NUMBER_HPP

#include "shard/string/trim.hpp"

#include <optional>
#include <string>

namespace shard {
namespace string {

inline std::optional<int> to_int(const std::string& str) {
    try {
        std::size_t idx;
        if (auto value = std::stoi(str, &idx, 10); idx == str.length()) {
            return value;
        }
    } catch (...) {
        // not an integer
    }
    return std::nullopt;
}

inline std::optional<unsigned long> to_unsigned(const std::string& str) {
    auto str_trimmed = ltrim_copy(str);
    if (!str_trimmed.empty() && str_trimmed[0] == '-') {
        return std::nullopt;
    }

    try {
        std::size_t idx;
        if (auto value = std::stoul(str, &idx, 10); idx == str.length()) {
            return value;
        }
    } catch (...) {
        // not an unsigned
    }
    return std::nullopt;
}

inline std::optional<float> to_float(const std::string& str) {
    try {
        std::size_t idx;
        if (auto value = std::stof(str, &idx); idx == str.length()) {
            return value;
        }
    } catch (...) {
        // not a float
    }
    return std::nullopt;
}

inline std::optional<double> to_double(const std::string& str) {
    try {
        std::size_t idx;
        if (auto value = std::stod(str, &idx); idx == str.length()) {
            return value;
        }
    } catch (...) {
        // not a double
    }
    return std::nullopt;
}

} // namespace string

// bring symbols into parent namespace

using string::to_double;
using string::to_float;
using string::to_int;
using string::to_unsigned;

} // namespace shard

#endif // SHARD_STRING_TO_NUMBER_HPP
