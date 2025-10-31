// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <atomic>
#include <cstddef>
#include <functional>

namespace shard {
namespace meta {
namespace detail {

struct default_typespace {};

} // namespace detail

// forward declaration
template <typename T>
class typespace;

/// Represents a unique ID for a type
template <typename T>
class type_id {
    friend class typespace<T>;

public:
    /// Get the underlying value of the type ID
    std::size_t value() const { return m_value; }

    /// Conversion operator to the underlying value
    /* implicit */ operator std::size_t() const /* NOLINT */ { return value(); }

private:
    explicit type_id(std::size_t value)
    : m_value(value) {}

private:
    std::size_t m_value;
};

/// Represents a family of type IDs
template <typename T>
class typespace {
public:
    /// Get the unique ID for a type
    template <typename>
    static type_id<T> id() {
        static type_id<T> id(s_counter++);
        return id;
    }

private:
    static std::atomic<std::size_t> s_counter;
};

template <typename T>
std::atomic<std::size_t> typespace<T>::s_counter = ATOMIC_VAR_INIT(0);

// operators

template <typename T>
bool operator==(const type_id<T>& lhs, const type_id<T>& rhs) {
    return lhs.value() == rhs.value();
}

template <typename T>
bool operator!=(const type_id<T>& lhs, const type_id<T>& rhs) {
    return !(lhs == rhs);
}

template <typename T>
bool operator<(const type_id<T>& lhs, const type_id<T>& rhs) {
    return lhs.value() < rhs.value();
}

template <typename T>
bool operator<=(const type_id<T>& lhs, const type_id<T>& rhs) noexcept {
    return !(rhs < lhs);
}

template <typename T>
bool operator>(const type_id<T>& lhs, const type_id<T>& rhs) noexcept {
    return rhs < lhs;
}

template <typename T>
bool operator>=(const type_id<T>& lhs, const type_id<T>& rhs) noexcept {
    return !(lhs < rhs);
}

} // namespace meta

// bring symbols into parent namespace

using meta::type_id;

} // namespace shard

// std::hash compatibility

template <typename T>
struct std::hash<shard::meta::type_id<T>> {
    std::size_t operator()(const shard::meta::type_id<T>& type_id) const noexcept {
        return hash<std::size_t> {}(type_id.value());
    }
};

#define SHARD_INTERNAL_TYPEID_1(T) shard::meta::typespace<shard::meta::detail::default_typespace>::id<T>()
#define SHARD_INTERNAL_TYPEID_2(N, T) shard::meta::typespace<N>::id<T>()

#define SHARD_INTERNAL_TYPEID_SELECT(_1, _2, MACRO, ...) MACRO
#define SHARD_TYPEID(...)                                                                                              \
    SHARD_INTERNAL_TYPEID_SELECT(__VA_ARGS__, SHARD_INTERNAL_TYPEID_2, SHARD_INTERNAL_TYPEID_1)(__VA_ARGS__)
