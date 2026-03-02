// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#pragma once

#include <cstdint>

#if defined(_MSC_VER)
#include <intrin.h>
#include <stdlib.h>
#endif

namespace shard::bit {

/// Common traits for bit information
template <typename>
struct traits {};

template <>
struct traits<std::uint64_t> {
    static std::uint64_t byteswap(std::uint64_t x) noexcept {
#if defined(_MSC_VER)
        return _byteswap_uint64(x);
#else
        return __builtin_bswap64(x);
#endif
    }

    static unsigned int popcount(std::uint64_t x) noexcept {
#if defined(_MSC_VER)
        return __popcnt64(x);
#else
        return __builtin_popcountll(x);
#endif
    }

    static unsigned int countr_zero(std::uint64_t x) noexcept {
#if defined(_MSC_VER)
        unsigned long index;
        return _BitScanForward64(&index, x);
        return index;
#else
        return __builtin_ctzll(x);
#endif
    }
};

template <>
struct traits<std::uint32_t> {
    static std::uint32_t byteswap(std::uint32_t x) noexcept {
#if defined(_MSC_VER)
        return _byteswap_ulong(x);
#else
        return __builtin_bswap32(x);
#endif
    }

    static unsigned int popcount(std::uint32_t x) noexcept {
#if defined(_MSC_VER)
        return __popcnt(x);
#else
        return __builtin_popcount(x);
#endif
    }

    static unsigned int countr_zero(std::uint32_t x) noexcept {
#if defined(_MSC_VER)
        unsigned long index;
        _BitScanForward(&index, static_cast<unsigned long>(x));
        return index;
#else
        return __builtin_ctz(x);
#endif
    }
};

template <>
struct traits<std::uint16_t> {
    static std::uint16_t byteswap(std::uint16_t x) noexcept {
#if defined(_MSC_VER)
        return _byteswap_ushort(x);
#else
        return __builtin_bswap16(x);
#endif
    }

    static unsigned int popcount(std::uint16_t x) noexcept {
#if defined(_MSC_VER)
        return __popcnt16(x);
#else
        return traits<std::uint32_t>::popcount(x);
#endif
    }

    static unsigned int countr_zero(std::uint16_t x) noexcept { return traits<std::uint32_t>::countr_zero(x); }
};

template <>
struct traits<std::uint8_t> {
    static std::uint8_t byteswap(std::uint8_t x) noexcept { return x; }

    static unsigned int popcount(std::uint8_t x) noexcept { return traits<std::uint32_t>::popcount(x); }

    static unsigned int countr_zero(std::uint8_t x) noexcept { return traits<std::uint32_t>::countr_zero(x); }
};

} // namespace shard::bit
