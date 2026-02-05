// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/bit/traits.hpp"

#include <type_traits>

namespace shard {
namespace bit {

/// Count trailing zeros
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
unsigned int countr_zero(T n) noexcept {
    if (n == 0) {
        return sizeof(T) * 8; // all zeros
    }
    return bit::traits<T>::countr_zero(n);
}

} // namespace bit

// bring symbols into parent namespace

using bit::countr_zero;

} // namespace shard
