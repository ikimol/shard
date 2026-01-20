// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#include "shard/uuid.hpp"

#include <shard/algorithm/enumerate.hpp>
#include <shard/crypto/md5.hpp>
#include <shard/crypto/sha1.hpp>
#include <shard/memory/data.hpp>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <ostream>
#include <vector>

namespace shard {
namespace {

bool needs_hyphen(std::size_t b) {
    return b == 3 || b == 5 || b == 7 || b == 9;
}

char to_char(std::uint8_t nibble) {
    constexpr char characters[] = "0123456789abcdef";
    assert(nibble <= 0xf);
    return characters[nibble & 0xf];
}

std::uint8_t to_nibble(const char ch) noexcept {
    if (ch >= '0' && ch <= '9') {
        return static_cast<std::uint8_t>(ch - '0');
    } else if (ch >= 'a' && ch <= 'f') {
        return static_cast<std::uint8_t>(10 + ch - 'a');
    } else if (ch >= 'A' && ch <= 'F') {
        return static_cast<std::uint8_t>(10 + ch - 'A');
    }
    return 0;
}

uuid generate_name_uuid(const uuid& ns, std::string_view name, crypto::hash_function_t hasher, std::uint8_t version) {
    // concatenate namespace bytes with name bytes
    std::vector<uuid::value_type> buffer;
    buffer.reserve(16 + name.size());
    for (std::size_t i = 0; i < 16; ++i) {
        buffer.push_back(ns.bytes()[i]);
    }
    for (auto c : name) {
        buffer.push_back(static_cast<uuid::value_type>(c));
    }

    // compute hash
    auto hash = hasher(data(buffer.data(), buffer.size()));
    auto digest = reinterpret_cast<const uuid::value_type*>(hash.digest().bytes());

    // copy first 16 bytes of hash
    std::array<uuid::value_type, 16> result_data {};
    std::copy_n(digest, 16, result_data.begin());

    // set version and variant
    result_data[6] = (result_data[6] & 0x0f) | version;
    result_data[8] = (result_data[8] & 0x3f) | 0x80;

    return uuid(result_data);
}

} // namespace

uuid uuid::nil = uuid({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

uuid uuid::max = uuid({0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff});

uuid uuid::namespace_dns
    = uuid({0x6b, 0xa7, 0xb8, 0x10, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8});

uuid uuid::namespace_url
    = uuid({0x6b, 0xa7, 0xb8, 0x11, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8});

uuid uuid::namespace_oid
    = uuid({0x6b, 0xa7, 0xb8, 0x12, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8});

uuid uuid::namespace_x500
    = uuid({0x6b, 0xa7, 0xb8, 0x14, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8});

uuid::uuid(const value_type (&data)[16]) noexcept {
    std::copy(std::cbegin(data), std::cend(data), std::begin(m_data));
}

constexpr uuid::uuid(const std::array<value_type, 16>& data) noexcept
: m_data(data) {}

std::optional<uuid> uuid::from_string(std::string_view str) noexcept {
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

bool uuid::is_nil() const {
    return std::all_of(std::cbegin(m_data), std::cend(m_data), [](auto i) { return i == 0; });
}

uuid::version uuid::get_version() const {
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

uuid::variant uuid::get_variant() const {
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

std::string uuid::to_string() const {
    std::vector<char> buffer(36);
    std::size_t index = 0;
    for (auto&& [i, b] : enumerate(m_data)) {
        buffer[index++] = to_char((b >> 4) & 0xf);
        buffer[index++] = to_char((b >> 0) & 0xf);
        if (needs_hyphen(i)) {
            buffer[index++] = '-';
        }
    }
    return {buffer.data(), buffer.size()};
}

uuid uuid::make_v3_uuid(const uuid& ns, std::string_view name) {
    return generate_name_uuid(ns, name, &crypto::md5, 0x30);
}

uuid uuid::make_v5_uuid(const uuid& ns, std::string_view name) {
    return generate_name_uuid(ns, name, &crypto::sha1, 0x50);
}

void swap(uuid& lhs, uuid& rhs) noexcept {
    lhs.swap(rhs);
}

std::ostream& operator<<(std::ostream& os, const uuid& u) {
    os << u.to_string();
    return os;
}

bool operator==(const uuid& lhs, const uuid& rhs) noexcept {
    return lhs.m_data == rhs.m_data;
}

bool operator<(const uuid& lhs, const uuid& rhs) noexcept {
    return lhs.m_data < rhs.m_data;
}

} // namespace shard
