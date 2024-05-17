// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <memory>

namespace shard {
namespace memory {

template <typename T, auto DeleteFunction>
class managed_ptr {
public:
    using pointer = T*;
    using element_type = T;
    using deleter_type = decltype(DeleteFunction);

public:
    /// Default constructor
    constexpr managed_ptr() noexcept
    : m_ptr(nullptr, DeleteFunction) {}

    /// Create using a raw pointer
    explicit managed_ptr(pointer ptr) noexcept
    : m_ptr(ptr, DeleteFunction) {}

    /// Get the underlying raw pointer
    pointer get() const noexcept { return m_ptr.get(); }

    /// Release the underlying raw pointer and return it
    pointer release() noexcept { return m_ptr.release(); }

    /// Destroy the underlying raw pointer
    void reset(pointer ptr = pointer()) noexcept { m_ptr.reset(ptr); }

    /// Check if the underlying raw pointer is not null
    explicit operator bool() const { return m_ptr != nullptr; }

private:
    std::unique_ptr<element_type, deleter_type> m_ptr;
};

// helpers

template <typename T, auto DeleteFunction>
managed_ptr<T, DeleteFunction> make_managed(T* ptr) noexcept {
    return managed_ptr<T, DeleteFunction>(ptr);
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
