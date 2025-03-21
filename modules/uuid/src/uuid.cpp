// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#include "shard/uuid.hpp"

#include <shard/algorithm/enumerate.hpp>

#include <algorithm>
#include <cassert>
#include <cctype>

namespace shard {
namespace {

bool needs_hyphen(std::uint8_t b) {
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

} // namespace

uuid::uuid(const value_type (&data)[16]) noexcept {
    std::copy(std::cbegin(data), std::cend(data), std::begin(m_data));
}

constexpr uuid::uuid(const std::array<value_type, 16>& data) noexcept
: m_data(data) {}

std::optional<uuid> uuid::from_string(const std::string_view& str) noexcept {
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

bool uuid::is_null() const {
    return std::all_of(std::cbegin(m_data), std::cend(m_data), [](auto i) { return i == 0; });
}

enum uuid::version uuid::version() const {
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

enum uuid::variant uuid::variant() const {
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

} // namespace shard
