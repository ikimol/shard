// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/hash_value.hpp>

#include <array>
#include <cstdint>
#include <iosfwd>
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

    // predefined UUIDs

    static uuid nil; ///< UUID with all bits set to '0'
    static uuid max; ///< UUID with all bits set to '1'

    // predefined namespace IDs for v3 and v5 UUIDs

    static uuid namespace_dns;  ///< The name is a fully-qualified domain name
    static uuid namespace_url;  ///< The name is a URL
    static uuid namespace_oid;  ///< The name is an ISO OID
    static uuid namespace_x500; ///< The name is an X.500 DN in DER or a text output format

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
    static std::optional<uuid> from_string(std::string_view str) noexcept;

    /// Check if every byte of the UUID is zero
    bool is_nil() const;

    /// Get the version of the UUID
    version get_version() const;

    /// Get the variant of the UUID
    variant get_variant() const;

    /// Convert the UUID to its string representation
    std::string to_string() const;

    /// Get the raw byte array
    ///
    /// \note The size of the array is guaranteed to be 16.
    const value_type* bytes() const { return m_data.data(); }

    /// Swap the value of the UUID with another
    void swap(uuid& other) noexcept { m_data.swap(other.m_data); }

    // generators

    /// Get a new UUID created by the system
    static uuid make_system_uuid();

    /// Get a new UUID V3 by hashing the given namespace and name using MD5
    static uuid make_v3_uuid(const uuid& ns, std::string_view name);

    /// Get a new UUID V5 by hashing the given namespace and name using SHA-1
    static uuid make_v5_uuid(const uuid& ns, std::string_view name);

private:
    alignas(8) std::array<value_type, 16> m_data = {};
};

// operators

void swap(uuid& lhs, uuid& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const uuid& u);

bool operator==(const uuid& lhs, const uuid& rhs) noexcept;

bool operator<(const uuid& lhs, const uuid& rhs) noexcept;

// implementation

template <typename Iterator>
uuid::uuid(Iterator first, Iterator last) {
    if (std::distance(first, last) == 16) {
        std::copy(first, last, std::begin(m_data));
    }
}

} // namespace shard

// std::hash compatibility

template <>
struct std::hash<shard::uuid> {
    std::size_t operator()(const shard::uuid& id) const noexcept {
        auto hi = *reinterpret_cast<const std::uint64_t*>(&id.m_data[0]);
        auto lo = *reinterpret_cast<const std::uint64_t*>(&id.m_data[8]);
        return shard::hash_value(hi, lo);
    }
};
