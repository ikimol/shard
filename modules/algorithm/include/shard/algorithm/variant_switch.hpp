// Copyright (c) 2022 Miklos Molnar. All rights reserved.

#ifndef SHARD_ALGORITHM_VARIANT_SWITCH_HPP
#define SHARD_ALGORITHM_VARIANT_SWITCH_HPP

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
    std::visit(detail::overloaded {SHARD_FWD(ts)...}, SHARD_FWD(variant));
}

} // namespace algorithm

// bring symbols into parent namespace

using algorithm::variant_switch;

} // namespace shard

#endif // SHARD_ALGORITHM_VARIANT_SWITCH_HPP
