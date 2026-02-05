// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/bit/traits.hpp"

#include <type_traits>

namespace shard {
namespace bit {

/// Swap the byte order of some scalar value
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
T byteswap(T x) {
    return bit::traits<T>::byteswap(x);
}

} // namespace bit

// bring symbols into parent namespace

using bit::byteswap;

} // namespace shard
