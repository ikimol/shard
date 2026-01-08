// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#pragma once

#include <cstdint>
#include <type_traits>

#if defined(_MSC_VER)
#include <stdlib.h>
#endif

namespace shard {
namespace bit {
namespace detail {

template <typename>
struct swap_byte_order_impl {};

template <>
struct swap_byte_order_impl<std::uint16_t> {
    static std::uint16_t swap(std::uint16_t x) noexcept {
#if defined(_MSC_VER)
        return _byteswap_ushort(x);
#else
        return __builtin_bswap16(x);
#endif
    }
};

template <>
struct swap_byte_order_impl<std::uint32_t> {
    static std::uint32_t swap(std::uint32_t x) noexcept {
#if defined(_MSC_VER)
        return _byteswap_ulong(x);
#else
        return __builtin_bswap32(x);
#endif
    }
};

template <>
struct swap_byte_order_impl<std::uint64_t> {
    static std::uint64_t swap(std::uint64_t x) noexcept {
#if defined(_MSC_VER)
        return _byteswap_uint64(x);
#else
        return __builtin_bswap64(x);
#endif
    }
};

} // namespace detail

/// Swap the byte order of some scalar value
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
T byteswap(T x) {
    return detail::swap_byte_order_impl<T>::swap(x);
}

} // namespace bit

// bring symbols into parent namespace

using bit::byteswap;

} // namespace shard
