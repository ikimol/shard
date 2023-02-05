// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_UTILITY_UUID_HPP
#define SHARD_UTILITY_UUID_HPP

#include <shard/algorithm/enumerate.hpp>
#include <shard/algorithm/stl_wrappers.hpp>
#include <shard/utility/hash_value.hpp>

#include <array>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <optional>
#include <string>

namespace shard {

class uuid {
    friend std::hash<uuid>;
    friend bool operator==(const uuid&, const uuid&) noexcept;
    friend bool operator<(const uuid&, const uuid&) noexcept;

public:
    using value_type = std::uint8_t;

    /// The UUID variant is indicated by a bit pattern in octet 8
    enum class variant {
        ncs,
        rfc,
        microsoft,
        reserved,
    };

    /// The UUID variant is indicated by a bit pattern in octet 6
    enum class version {
        none = 0,                ///< Version of nil / invalid UUIDs
        time_based = 1,          ///< The time-based version
        dce_security = 2,        ///< DCE security version
        name_based_md5 = 3,      ///< The name-based version using MD5
        random_number_based = 4, ///< The randomly generated version
        name_based_sha1 = 5,     ///< The name-based version using SHA-1
    };

public:
    /// Default constructor
    uuid() noexcept = default;

    /// Create a UUID from a raw array of bytes
    explicit uuid(value_type (&data)[16]) noexcept {
        std::copy(std::cbegin(data), std::cend(data), std::begin(m_data));
    }

    /// Create a UUID from an array of bytes
    constexpr explicit uuid(const std::array<value_type, 16>& data) noexcept : m_data(data) {}

    /// Create a UUID from a pair of iterators
    template <typename Iterator>
    explicit uuid(Iterator first, Iterator last) {
        if (std::distance(first, last) == 16) {
            std::copy(first, last, std::begin(m_data));
        }
    }

    /// Create a UUID from its string representation
    static std::optional<uuid> from_string(const std::string& str) noexcept {
        if (str.empty()) {
            return std::nullopt;
        }

        std::size_t has_braces = 0;
        if (str.front() == '{') {
            has_braces = 1;
        }
        if (has_braces && str.back() != '}') {
            return std::nullopt;
        }

        std::array<std::uint8_t, 16> data = {0};

        bool first_digit = true;
        std::size_t index = 0;

        for (std::size_t i = has_braces; i < str.size() - has_braces; ++i) {
            if (str[i] == '-') {
                continue;
            }

            if (index >= 16 || !std::isxdigit(str[i])) {
                return std::nullopt;
            }

            if (first_digit) {
                data[index] = to_nibble(str[i]) << 4;
                first_digit = false;
            } else {
                data[index] = data[index] | to_nibble(str[i]);
                index++;
                first_digit = true;
            }
        }

        if (index < 16) {
            return std::nullopt;
        }

        return uuid(data);
    }

    /// Check if every byte of the UUID is zero
    bool is_null() const {
        return shard::any_of(m_data, [](std::uint8_t i) {
            return i != 0;
        });
    }

    /// Get the version of the UUID
    version version() const {
        if ((m_data[6] & 0xf0) == 0x10) {
            return version::time_based;
        } else if ((m_data[6] & 0xf0) == 0x20) {
            return version::dce_security;
        } else if ((m_data[6] & 0xf0) == 0x30) {
            return version::name_based_md5;
        } else if ((m_data[6] & 0xf0) == 0x40) {
            return version::random_number_based;
        } else if ((m_data[6] & 0xf0) == 0x50) {
            return version::name_based_sha1;
        } else {
            return version::none;
        }
    }

    /// Get the variant of the UUID
    variant variant() const {
        if ((m_data[8] & 0x80) == 0x00) {
            return variant::ncs;
        } else if ((m_data[8] & 0xc0) == 0x80) {
            return variant::rfc;
        } else if ((m_data[8] & 0xe0) == 0xc0) {
            return variant::microsoft;
        } else {
            return variant::reserved;
        }
    }

    /// Convert the UUID to its string representation
    std::string to_string() const {
        std::string buffer(36, '#');
        std::size_t index = 0;
        for (auto [i, b] : enumerate(m_data)) {
            buffer[index++] = to_char((b >> 4) & 0xf);
            buffer[index++] = to_char((b >> 0) & 0xf);
            if (needs_hyphen(i)) {
                buffer[index++] = '-';
            }
        }
        return buffer;
    }

    /// Swap the value of the UUID with another
    void swap(uuid& other) noexcept { m_data.swap(other.m_data); }

    // generators

    /// Get a new UUID created by the system
    static uuid make_system_uuid();

private:
    static bool needs_hyphen(std::uint8_t b) { return b == 3 || b == 5 || b == 7 || b == 9; }

    static char to_char(std::uint8_t nibble) {
        constexpr char characters[] = "0123456789abcdef";
        assert(nibble >= 0x0 && nibble <= 0xf);
        return characters[nibble & 0xf];
    }

    static std::uint8_t to_nibble(const char ch) noexcept {
        if (ch >= '0' && ch <= '9') {
            return static_cast<std::uint8_t>(ch - '0');
        } else if (ch >= 'a' && ch <= 'f') {
            return static_cast<std::uint8_t>(10 + ch - 'a');
        } else if (ch >= 'A' && ch <= 'F') {
            return static_cast<std::uint8_t>(10 + ch - 'A');
        }
        return 0;
    }

private:
    alignas(8) std::array<value_type, 16> m_data = {0};
};

inline void swap(uuid& lhs, uuid& rhs) noexcept {
    lhs.swap(rhs);
}

inline std::ostream& operator<<(std::ostream& os, const uuid& u) {
    os << u.to_string();
    return os;
}

inline bool operator==(const uuid& lhs, const uuid& rhs) noexcept {
    return lhs.m_data == rhs.m_data;
}

inline bool operator<(const uuid& lhs, const uuid& rhs) noexcept {
    return lhs.m_data < rhs.m_data;
}

} // namespace shard

template <>
struct std::hash<shard::uuid> {
    std::size_t operator()(const shard::uuid& id) const noexcept {
        auto hi = *reinterpret_cast<const std::uint64_t*>(&id.m_data[0]);
        auto lo = *reinterpret_cast<const std::uint64_t*>(&id.m_data[8]);
        return shard::hash_value(hi, lo);
    }
};

#endif // SHARD_UTILITY_UUID_HPP
