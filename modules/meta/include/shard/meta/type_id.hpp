// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <atomic>
#include <cstdint>
#include <functional>

namespace shard {
namespace meta {
namespace detail {

struct default_typespace {};

} // namespace detail

/// Represents a unique ID for a type
template <typename T, typename>
class type_id {
public:
    using value_type = T;

public:
    /// Convert a previously assigned number back to a type ID
    explicit constexpr type_id(value_type value)
    : m_value(value) {}

    /// Get the underlying value of the type ID
    constexpr value_type value() const { return m_value; }

    /// Conversion operator to the underlying value
    /* implicit */ constexpr operator value_type() const /* NOLINT */ { return value(); }

private:
    value_type m_value;
};

/// Represents a family of type IDs
template <typename T, typename U>
class typespace {
public:
    using value_type = T;
    using id_type = type_id<T, U>;

public:
    /// Get the unique ID for a type
    template <typename>
    static id_type id() {
        static id_type id(s_counter.fetch_add(1, std::memory_order_relaxed));
        return id;
    }

private:
    static std::atomic<value_type> s_counter;
};

template <typename T, typename S>
std::atomic<T> typespace<T, S>::s_counter = ATOMIC_VAR_INIT(0);

// operators

template <typename T, typename U>
bool operator==(const type_id<T, U>& lhs, const type_id<T, U>& rhs) {
    return lhs.value() == rhs.value();
}

template <typename T, typename U>
bool operator!=(const type_id<T, U>& lhs, const type_id<T, U>& rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
bool operator<(const type_id<T, U>& lhs, const type_id<T, U>& rhs) {
    return lhs.value() < rhs.value();
}

template <typename T, typename U>
bool operator<=(const type_id<T, U>& lhs, const type_id<T, U>& rhs) noexcept {
    return !(rhs < lhs);
}

template <typename T, typename U>
bool operator>(const type_id<T, U>& lhs, const type_id<T, U>& rhs) noexcept {
    return rhs < lhs;
}

template <typename T, typename U>
bool operator>=(const type_id<T, U>& lhs, const type_id<T, U>& rhs) noexcept {
    return !(lhs < rhs);
}

} // namespace meta

// bring symbols into parent namespace

using meta::type_id;

} // namespace shard

// std::hash compatibility

template <typename T, typename U>
struct std::hash<shard::meta::type_id<T, U>> {
    std::size_t operator()(const shard::meta::type_id<T, U>& id) const noexcept {
        return hash<std::size_t> {}(id.value());
    }
};

#define SHARD_INTERNAL_TYPEID_1(T)                                                                                     \
    shard::meta::typespace<std::uint16_t, shard::meta::detail::default_typespace>::id<T>()
#define SHARD_INTERNAL_TYPEID_2(ID_T, T) shard::meta::typespace<ID_T::value_type, ID_T>::id<T>()

#define SHARD_INTERNAL_TYPEID_SELECT(_1, _2, MACRO, ...) MACRO
#define SHARD_TYPEID(...)                                                                                              \
    SHARD_INTERNAL_TYPEID_SELECT(__VA_ARGS__, SHARD_INTERNAL_TYPEID_2, SHARD_INTERNAL_TYPEID_1)(__VA_ARGS__)
