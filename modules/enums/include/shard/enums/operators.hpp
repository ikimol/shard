// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/enums/traits.hpp"

namespace shard::enums::operators {

// implements lhs != rhs
template <typename E, typename = std::enable_if_t<std::is_enum<E>::value>>
constexpr bool operator!=(const E& lhs, const std::underlying_type_t<E>& rhs) noexcept {
    return to_underlying(lhs) != rhs;
}

template <typename E, typename = std::enable_if_t<std::is_enum<E>::value>>
constexpr bool operator!=(const std::underlying_type_t<E>& lhs, const E& rhs) noexcept {
    return lhs != to_underlying(rhs);
}

// implements lhs == rhs

template <typename E, typename = std::enable_if_t<std::is_enum<E>::value>>
constexpr bool operator==(const E& lhs, const std::underlying_type_t<E>& rhs) noexcept {
    return to_underlying(lhs) == rhs;
}

template <typename E, typename = std::enable_if_t<std::is_enum<E>::value>>
constexpr bool operator==(const std::underlying_type_t<E>& lhs, const E& rhs) noexcept {
    return lhs == to_underlying(rhs);
}

} // namespace shard::enums::operators
