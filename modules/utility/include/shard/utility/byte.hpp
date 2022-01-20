// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_UTILITY_BYTE_HPP
#define SHARD_UTILITY_BYTE_HPP

#include <type_traits>

namespace shard {
namespace core {

enum class byte : unsigned char {};

template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
inline constexpr byte to_byte(T value) noexcept {
    return static_cast<byte>(value);
}

template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
constexpr T to_integer(byte b) noexcept {
    return static_cast<T>(b);
}

template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
constexpr byte& operator<<=(byte& b, T shift) noexcept {
    return b = b << shift;
}

template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
constexpr byte& operator>>=(byte& b, T shift) noexcept {
    return b = b >> shift;
}

template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
constexpr byte operator<<(byte b, T shift) noexcept {
    return byte(static_cast<unsigned int>(b) << shift);
}

template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
constexpr byte operator>>(byte b, T shift) noexcept {
    return byte(static_cast<unsigned int>(b) >> shift);
}

constexpr byte operator|(byte lhs, byte rhs) noexcept {
    return byte(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs));
}

constexpr byte operator&(byte lhs, byte rhs) noexcept {
    return byte(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs));
}

constexpr byte operator^(byte lhs, byte rhs) noexcept {
    return byte(static_cast<unsigned int>(lhs) ^ static_cast<unsigned int>(rhs));
}

constexpr byte& operator|=(byte& lhs, byte rhs) noexcept {
    return lhs = lhs | rhs;
}

constexpr byte& operator&=(byte& lhs, byte rhs) noexcept {
    return lhs = lhs & rhs;
}

constexpr byte& operator^=(byte& lhs, byte rhs) noexcept {
    return lhs = lhs ^ rhs;
}

constexpr byte operator~(byte b) noexcept {
    return byte(~static_cast<unsigned int>(b));
}

} // namespace core

// bring symbols into parent namespace

using core::byte;
using core::to_byte;
using core::to_integer;

} // namespace shard

#endif // SHARD_UTILITY_BYTE_HPP
