// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <cstring>
#include <new>
#include <utility>

namespace shard {
namespace memory {

/// Manages an object inside a pre-allocated buffer
template <typename T>
class buffer_ptr {
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

public:
    /// Create a new object inside the given buffer
    template <typename... Args>
    explicit buffer_ptr(std::byte* buffer, Args&&... args)
    : m_buffer(buffer) {
        new (buffer) T(std::forward<Args>(args)...);
    }

    // copy and move construction is disabled
    buffer_ptr(const buffer_ptr& other) = delete;
    buffer_ptr(buffer_ptr&& other) = delete;

    /// Destroys the object and nulls out the original buffer
    ~buffer_ptr() {
        ptr()->~T();
        std::memset(m_buffer, '\0', sizeof(T));
    }

    /// Access the underlying pointer
    reference operator*() { return *ptr(); }

    /// Access the underlying pointer
    const_reference operator*() const { return *ptr(); }

    /// Access the underlying pointer
    pointer operator->() noexcept { return ptr(); }

    /// Access the underlying pointer
    const_pointer operator->() const noexcept { return ptr(); }

    /// Get the underlying pointer
    pointer get() noexcept { return ptr(); }

    /// Get the underlying pointer
    const_pointer get() const noexcept { return ptr(); }

private:
    pointer ptr() const { return reinterpret_cast<pointer>(m_buffer); }

private:
    std::byte* m_buffer;
};

} // namespace memory

// bring symbols into parent namespace

using memory::buffer_ptr;

} // namespace shard
