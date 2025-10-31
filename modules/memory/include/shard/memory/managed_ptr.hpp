// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <memory>

namespace shard {
namespace memory {

/// Represents a wrapper for a pointer owned by some other component
template <typename T, auto DeleteFunction>
class managed_ptr {
public:
    using element_type = T;
    using pointer = element_type*;
    using reference = element_type&;
    using deleter_type = decltype(DeleteFunction);

public:
    /// Default constructor
    constexpr managed_ptr() noexcept
    : m_ptr(nullptr, DeleteFunction) {}

    /// Create using a pointer
    explicit managed_ptr(pointer ptr) noexcept
    : m_ptr(ptr, DeleteFunction) {}

    /// Converting constructor
    template <typename U, auto OtherDeleteFunction>
    /* implicit */ managed_ptr(managed_ptr<U, OtherDeleteFunction> other) noexcept /* NOLINT */
    : m_ptr(other.m_ptr, OtherDeleteFunction) {}

    /// Access the underlying pointer
    reference operator*() const { return *m_ptr; }

    /// Access the underlying pointer
    pointer operator->() const noexcept { return m_ptr.get(); }

    /// Get the underlying pointer
    pointer get() const noexcept { return m_ptr.get(); }

    /// Check if the underlying pointer is not null
    explicit operator bool() const noexcept { return m_ptr != nullptr; }

    /// Pointer conversion operator
    explicit operator pointer() const noexcept { return m_ptr; }

    /// Release the underlying pointer and return it
    pointer release() noexcept { return m_ptr.release(); }

    /// Change the underlying pointer
    void reset(pointer ptr = pointer()) noexcept { m_ptr.reset(ptr); }

    /// Swap with another managed pointer
    void swap(managed_ptr& other) noexcept {
        using std::swap;
        swap(m_ptr, other.m_ptr);
    }

private:
    std::unique_ptr<element_type, deleter_type> m_ptr;
};

// helpers

template <typename T, auto DeleteFunction>
managed_ptr<T, DeleteFunction> make_managed(T* ptr) noexcept {
    return managed_ptr<T, DeleteFunction>(ptr);
}

template <typename T, auto DeleteFunction>
void swap(managed_ptr<T, DeleteFunction>& lhs, managed_ptr<T, DeleteFunction>& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace memory

// bring symbols into parent namespace

using memory::make_managed;
using memory::managed_ptr;

} // namespace shard

// std::hash compatibility

namespace std {

template <typename T, auto DeleteFunction>
struct hash<shard::managed_ptr<T, DeleteFunction>> {
    std::size_t operator()(const shard::managed_ptr<T, DeleteFunction>& ptr) const {
        return std::hash<T*> {}(ptr.get());
    }
};

template <typename T, auto DeleteFunction>
struct hash<const shard::managed_ptr<T, DeleteFunction>> {
    std::size_t operator()(const shard::managed_ptr<T, DeleteFunction>& ptr) const {
        return std::hash<T*> {}(ptr.get());
    }
};

} // namespace std
