// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/hash_value.hpp"

#include <shard/string/fmt.hpp>

#include <array>
#include <functional>
#include <optional>
#include <string>

namespace shard {

template <typename T>
class basic_version {
public:
    using value_type = T;

public:
    /// Create the a version with 0 as its elements
    constexpr basic_version() noexcept = default;

    /// Create a version from the given elements
    constexpr basic_version(value_type major, value_type minor, value_type patch) noexcept {
        m_values[0] = major;
        m_values[1] = minor;
        m_values[2] = patch;
    }

    /// Try to create a version from a string
    static std::optional<basic_version> from_string(std::string_view v) noexcept {
        basic_version result;

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

            if (!parse_int(s, *it++)) {
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
    static bool parse_int(std::string_view s, value_type& out) noexcept {
        if (s.empty()) {
            return false;
        }
        value_type value = 0;
        for (auto ch : s) {
            if (ch < '0' || ch > '9') {
                return false;
            }
            value = static_cast<value_type>(value * 10 + (ch - '0'));
        }
        out = value;
        return true;
    }

private:
    std::array<value_type, 3> m_values = {};
};

/// Represents a version number
using version = basic_version<int>;

// operators

template <typename T>
bool operator==(const basic_version<T>& lhs, const basic_version<T>& rhs) {
    return lhs.major() == rhs.major() && lhs.minor() == rhs.minor() && lhs.patch() == rhs.patch();
}

template <typename T>
bool operator!=(const basic_version<T>& lhs, const basic_version<T>& rhs) {
    return !(lhs == rhs);
}

template <typename T>
bool operator<(const basic_version<T>& lhs, const basic_version<T>& rhs) {
    if (lhs.major() != rhs.major()) {
        return lhs.major() < rhs.major();
    } else if (lhs.minor() != rhs.minor()) {
        return lhs.minor() < rhs.minor();
    } else {
        return lhs.patch() < rhs.patch();
    }
}

template <typename T>
bool operator<=(const basic_version<T>& lhs, const basic_version<T>& rhs) {
    return !(rhs < lhs);
}

template <typename T>
bool operator>(const basic_version<T>& lhs, const basic_version<T>& rhs) {
    return rhs < lhs;
}

template <typename T>
bool operator>=(const basic_version<T>& lhs, const basic_version<T>& rhs) {
    return !(lhs < rhs);
}

} // namespace shard

// std::hash compatibility

template <typename T>
struct std::hash<shard::basic_version<T>> {
    std::size_t operator()(const shard::basic_version<T>& v) const noexcept {
        return shard::hash_value(v.major(), v.minor(), v.patch());
    }
};
