// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <atomic>
#include <cstddef>

namespace shard::meta {
namespace detail {

struct default_typespace {};

} // namespace detail

/// Represents the unique id of a type inside a typespace
using typeid_t = std::size_t;

/// Represents a family of type ids
template <typename>
class typespace {
public:
    /// Get the unique id
    template <typename>
    static typeid_t id() {
        static const auto id = s_counter++;
        return id;
    }

private:
    static std::atomic<typeid_t> s_counter;
};

template <typename T>
std::atomic<typeid_t> typespace<T>::s_counter = ATOMIC_VAR_INIT(0);

} // namespace shard::meta

#define SHARD_INTERNAL_TYPEID_1(T) shard::meta::typespace<shard::meta::detail::default_typespace>::id<T>()
#define SHARD_INTERNAL_TYPEID_2(T, N) shard::meta::typespace<T>::id<N>()

#define SHARD_INTERNAL_TYPEID_SELECT(_1, _2, MACRO, ...) MACRO
#define SHARD_TYPEID(...)                                                                                              \
    SHARD_INTERNAL_TYPEID_SELECT(__VA_ARGS__, SHARD_INTERNAL_TYPEID_2, SHARD_INTERNAL_TYPEID_1)(__VA_ARGS__)
