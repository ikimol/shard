// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/hash_value.hpp"

#include <shard/string/fmt.hpp>

#include <array>
#include <charconv>
#include <functional>
#include <optional>
#include <string>

namespace shard {

class version {
public:
    using value_type = int;

public:
    /// Create the a version with 0 as its elements
    constexpr version() noexcept = default;

    /// Create a version from the given elements
    constexpr version(value_type major, value_type minor, value_type patch) noexcept {
        m_values[0] = major;
        m_values[1] = minor;
        m_values[2] = patch;
    }

    /// Try to create a version from a string
    static std::optional<version> from_string(std::string_view v) noexcept {
        version result;

        // strip build metadata if present (e.g. '+001')
        if (auto i = v.find('+'); i != std::string_view::npos) {
            v = v.substr(0, i);
        }

        // strip pre-release label if present (e.g. '-alpha')
        if (auto i = v.find('-'); i != std::string_view::npos) {
            v = v.substr(0, i);
        }

        std::size_t i = 0;
        std::size_t j = 0;
        auto it = result.m_values.begin();

        while (j != std::string_view::npos) {
            j = v.find('.', i);

            std::string_view s;
            if (j != std::string_view::npos) {
                s = v.substr(i, j - i);
                i = j + 1;
            } else {
                s = v.substr(i);
            }

            if (auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.length(), *it++); ec != std::errc()) {
                return std::nullopt;
            }
        }

        return result;
    }

    /// Get the major version number
    value_type major() const noexcept { return m_values[0]; }

    /// Get the minor version number
    value_type minor() const noexcept { return m_values[1]; }

    /// Get the patch version number
    value_type patch() const noexcept { return m_values[2]; }

    /// Convert the version to string
    std::string to_string() const noexcept { return shard::fmt("%d.%d.%d", m_values[0], m_values[1], m_values[2]); }

private:
    std::array<value_type, 3> m_values = {};
};

// operators

inline bool operator==(const version& lhs, const version& rhs) {
    return lhs.major() == rhs.major() && lhs.minor() == rhs.minor() && lhs.patch() == rhs.patch();
}

inline bool operator!=(const version& lhs, const version& rhs) {
    return !(lhs == rhs);
}

inline bool operator<(const version& lhs, const version& rhs) {
    if (lhs.major() != rhs.major()) {
        return lhs.major() < rhs.major();
    } else if (lhs.minor() != rhs.minor()) {
        return lhs.minor() < rhs.minor();
    } else {
        return lhs.patch() < rhs.patch();
    }
}

inline bool operator<=(const version& lhs, const version& rhs) {
    return !(rhs < lhs);
}

inline bool operator>(const version& lhs, const version& rhs) {
    return rhs < lhs;
}

inline bool operator>=(const version& lhs, const version& rhs) {
    return !(lhs < rhs);
}

} // namespace shard

// std::hash compatibility

template <>
struct std::hash<shard::version> {
    std::size_t operator()(const shard::version& v) const noexcept {
        return shard::hash_value(v.major(), v.minor(), v.patch());
    }
};
