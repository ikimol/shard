// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include <algorithm>
#include <array>
#include <exception>
#include <string_view>

namespace shard {
namespace enums {

/// Template to be specialized for enums
template <typename E>
struct enum_traits;

/*

template <>
struct enum_traits<EnumType> {
    static constexpr std::array values = {...};
    static constexpr std::array names = {...};
};

*/

/// Exception thrown when doing enum <-> string conversions
class incomplete_enum_traits final : public std::exception {
public:
    const char* what() const noexcept override { return "incomplete specialization of shard::enum_traits"; }
};

/// Get the name of an enum value
template <typename E>
std::string_view enum_name(E e) {
    using traits = enum_traits<E>;
    auto it = std::find(std::begin(traits::values), std::end(traits::values), e);
    if (it == std::end(traits::values)) {
        throw incomplete_enum_traits();
    }
    return traits::names[it - std::begin(traits::values)];
}

/// Get the enum value from its name
template <typename E>
E enum_value(std::string_view name) {
    using traits = enum_traits<E>;
    auto matcher = [name](const char* n) { return name == n; };
    auto it = std::find_if(std::begin(traits::names), std::end(traits::names), matcher);
    if (it == std::end(traits::names)) {
        throw incomplete_enum_traits();
    }
    return traits::values[it - std::begin(traits::names)];
}

} // namespace enums

// bring symbols into parent namespace

using enums::enum_name;
using enums::enum_traits;
using enums::enum_value;
using enums::incomplete_enum_traits;

} // namespace shard
