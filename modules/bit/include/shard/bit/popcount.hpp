// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/bit/traits.hpp"

#include <type_traits>

namespace shard {
namespace bit {

/// Count the number of set bits
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
unsigned int popcount(T n) noexcept {
    return bit::traits<T>::popcount(n);
}

} // namespace bit

// bring symbols into parent namespace

using bit::popcount;

} // namespace shard
