// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <atomic>
#include <cstddef>

namespace shard {
namespace meta {
namespace detail {

struct default_typespace {};

} // namespace detail

/// Represents a unique ID for a type
template <typename T>
struct type_id {
    /* implicit */ type_id(std::size_t value) /* NOLINT */
    : value(value) {}

    /* implicit */ operator std::size_t() const /* NOLINT */ { return value; }

    const std::size_t value;
};

/// Represents a family of type IDs
template <typename T>
class typespace {
public:
    /// Get the unique ID for a type
    template <typename>
    static type_id<T> id() {
        static const auto id = s_counter++;
        return id;
    }

private:
    static std::atomic<std::size_t> s_counter;
};

template <typename T>
std::atomic<std::size_t> typespace<T>::s_counter = ATOMIC_VAR_INIT(0);

} // namespace meta

// bring symbols into parent namespace

using meta::type_id;

} // namespace shard

#define SHARD_INTERNAL_TYPEID_1(T) shard::meta::typespace<shard::meta::detail::default_typespace>::id<T>()
#define SHARD_INTERNAL_TYPEID_2(T, N) shard::meta::typespace<T>::id<N>()

#define SHARD_INTERNAL_TYPEID_SELECT(_1, _2, MACRO, ...) MACRO
#define SHARD_TYPEID(...)                                                                                              \
    SHARD_INTERNAL_TYPEID_SELECT(__VA_ARGS__, SHARD_INTERNAL_TYPEID_2, SHARD_INTERNAL_TYPEID_1)(__VA_ARGS__)
