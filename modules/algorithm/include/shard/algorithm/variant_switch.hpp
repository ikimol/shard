// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/utility/preprocessor.hpp>

#include <variant>

namespace shard {
namespace algorithm {
namespace detail {

template <typename... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

} // namespace detail

/// Apply a functor based on the current value of the variant
template <typename... Ts, typename Variant>
auto variant_switch(Variant&& variant, Ts&&... ts) {
    return std::visit(detail::overloaded {std::forward<Ts>(ts)...}, std::forward<Variant>(variant));
}

} // namespace algorithm

// bring symbols into parent namespace

using algorithm::variant_switch;

} // namespace shard
