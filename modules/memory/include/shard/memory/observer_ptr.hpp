// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <utility>

namespace shard {
namespace memory {

/// A safe wrapper to be used instead of raw pointer
template <typename T>
class observer_ptr {
public:
    using element_type = T;
    using pointer = element_type*;
    using reference = element_type&;

public:
    /// Default constructor
    observer_ptr() noexcept = default;

    /// Raw pointer constructor
    explicit observer_ptr(pointer ptr) noexcept
    : m_ptr(ptr) {}

    /// Create using null
    explicit observer_ptr(std::nullptr_t) noexcept
    : m_ptr(nullptr) {}

    /// Converting constructor
    template <typename U>
    /* implicit */ observer_ptr(observer_ptr<U> other) noexcept /* NOLINT */
    : m_ptr(other.m_ptr) {}

    /// Access the underlying pointer
    reference operator*() const { return *m_ptr; }

    /// Access the underlying pointer
    pointer operator->() const noexcept { return m_ptr; }

    /// Get the underlying pointer
    pointer get() const noexcept { return m_ptr; }

    /// Check if the underlying pointer is not null
    explicit operator bool() const noexcept { return m_ptr != nullptr; }

    /// Pointer conversion operator
    explicit operator pointer() const noexcept { return m_ptr; }

    /// Release the underlying pointer and return it
    pointer release() noexcept { return std::exchange(m_ptr, nullptr); }

    /// Change the underlying pointer
    void reset(pointer ptr = nullptr) noexcept { m_ptr = ptr; }

    /// Swap with another observer
    void swap(observer_ptr& other) noexcept {
        using std::swap;
        swap(m_ptr, other.m_ptr);
    }

private:
    pointer m_ptr = nullptr;
};

// helpers

template <typename T>
observer_ptr<T> make_observer(T* ptr) noexcept {
    return observer_ptr<T>(ptr);
}

template <typename T>
void swap(observer_ptr<T>& lhs, observer_ptr<T>& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace memory

// bring symbols into parent namespace

using memory::make_observer;
using memory::observer_ptr;

} // namespace shard

// std::hash compatibility

namespace std {

template <typename T>
struct hash<shard::observer_ptr<T>> {
    std::size_t operator()(const shard::observer_ptr<T>& ptr) const { return std::hash<T*> {}(ptr.get()); }
};

template <typename T>
struct hash<const shard::observer_ptr<T>> {
    std::size_t operator()(const shard::observer_ptr<T>& ptr) const { return std::hash<T*> {}(ptr.get()); }
};

} // namespace std
