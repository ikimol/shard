// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/memory/managed_ptr.hpp"

namespace shard {
namespace memory {

/// Represents a wrapper for a pointer owned by some other component
template <typename T>
class unowned_ptr {
public:
    using element_type = T;
    using pointer = element_type*;
    using reference = element_type&;

public:
    /// Default constructor
    constexpr unowned_ptr() noexcept
    : m_ptr(nullptr) {}

    /// Create using a pointer
    explicit unowned_ptr(pointer ptr) noexcept
    : m_ptr(ptr) {}

    /// Converting constructor
    template <typename U, auto DeleteFunction>
    /* implicit */ unowned_ptr(const managed_ptr<U, DeleteFunction>& owner) noexcept /* NOLINT */
    : m_ptr(owner.get()) {}

    /// Access the underlying pointer
    reference operator*() const { return *m_ptr; }

    /// Access the underlying pointer
    pointer operator->() const noexcept { return m_ptr; }

    /// Get the underlying pointer
    pointer get() const noexcept { return m_ptr; }

    /// Check if the underlying pointer is not null
    explicit operator bool() const noexcept { return m_ptr != nullptr; }

    /// Change the underlying pointer
    void reset(pointer ptr = pointer()) noexcept { m_ptr = ptr; }

    /// Swap with another managed pointer
    void swap(unowned_ptr& other) noexcept {
        using std::swap;
        swap(m_ptr, other.m_ptr);
    }

private:
    pointer m_ptr;
};

// helpers

template <typename T>
unowned_ptr<T> make_unowned(T* ptr) noexcept {
    return unowned_ptr<T>(ptr);
}

template <typename T>
void swap(unowned_ptr<T>& lhs, unowned_ptr<T>& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace memory

// bring symbols into parent namespace

using memory::make_unowned;
using memory::unowned_ptr;

} // namespace shard

// std::hash compatibility

namespace std {

template <typename T>
struct hash<shard::unowned_ptr<T>> {
    std::size_t operator()(const shard::unowned_ptr<T>& ptr) const { return std::hash<T*> {}(ptr.get()); }
};

template <typename T>
struct hash<const shard::unowned_ptr<T>> {
    std::size_t operator()(const shard::unowned_ptr<T>& ptr) const { return std::hash<T*> {}(ptr.get()); }
};

} // namespace std
