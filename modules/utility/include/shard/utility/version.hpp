// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include <cstdint>
#include <functional>

namespace shard {
namespace utility {

struct version {
    /// Create the a version with 0 as its elements
    constexpr version() noexcept = default;

    /// Create a version from the given elements
    constexpr version(unsigned int major, unsigned int minor, unsigned int patch) noexcept
    : major(major)
    , minor(minor)
    , patch(patch) {}

    /// Get the value of the version by shifting each element
    std::uint32_t value() const { return major << 16 | minor << 8 | patch; }

    std::uint8_t major = 0;
    std::uint8_t minor = 0;
    std::uint8_t patch = 0;
};

// operators

inline bool operator==(const version& lhs, const version& rhs) {
    return lhs.major == rhs.major && lhs.minor == rhs.minor && lhs.patch == rhs.patch;
}

inline bool operator!=(const version& lhs, const version& rhs) {
    return !(lhs == rhs);
}

inline bool operator<(const version& lhs, const version& rhs) {
    if (lhs.major != rhs.major) {
        return lhs.major < rhs.major;
    } else if (lhs.minor != rhs.minor) {
        return lhs.minor < rhs.minor;
    } else {
        return lhs.patch < rhs.patch;
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

} // namespace utility

using utility::version;

} // namespace shard

// std::hash compatibility

template <>
struct std::hash<shard::version> {
    std::size_t operator()(const shard::version& v) const noexcept { return std::hash<std::uint32_t> {}(v.value()); }
};
