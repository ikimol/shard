// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/utility/hash_value.hpp>

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

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
    explicit uuid(const value_type (&data)[16]) noexcept;

    /// Create a UUID from an array of bytes
    constexpr explicit uuid(const std::array<value_type, 16>& data) noexcept;

    /// Create a UUID from a pair of iterators
    template <typename Iterator>
    explicit uuid(Iterator first, Iterator last);

    /// Create a UUID from its string representation
    static std::optional<uuid> from_string(const std::string_view& str) noexcept;

    /// Check if every byte of the UUID is zero
    bool is_null() const;

    /// Get the version of the UUID
    version version() const;

    /// Get the variant of the UUID
    variant variant() const;

    /// Convert the UUID to its string representation
    std::string to_string() const;

    /// Swap the value of the UUID with another
    void swap(uuid& other) noexcept { m_data.swap(other.m_data); }

    // generators

    /// Get a new UUID created by the system
    static uuid make_system_uuid();

private:
    alignas(8) std::array<value_type, 16> m_data = {};
};

// operators

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

// implementation

template <typename Iterator>
uuid::uuid(Iterator first, Iterator last) {
    if (std::distance(first, last) == 16) {
        std::copy(first, last, std::begin(m_data));
    }
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
