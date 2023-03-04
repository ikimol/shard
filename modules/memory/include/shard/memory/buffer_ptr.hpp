// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <cstring>
#include <new>
#include <utility>

namespace shard {
namespace memory {

template <typename T>
class buffer_ptr {
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

public:
    template <typename... Args>
    explicit buffer_ptr(std::byte* buffer, Args&&... args)
    : m_buffer(buffer) {
        new (buffer) T(std::forward<Args>(args)...);
    }

    buffer_ptr(const buffer_ptr& other) = delete;
    buffer_ptr(buffer_ptr&& other) = delete;

    ~buffer_ptr() {
        ptr()->~T();
        std::memset(m_buffer, '\0', sizeof(T));
    }

    reference operator*() { return *ptr(); }

    const_reference operator*() const { return *ptr(); }

    pointer operator->() { return ptr(); }

    const_pointer operator->() const { return ptr(); }

    pointer get() { return ptr(); }

    const_pointer get() const { return ptr(); }

private:
    inline pointer ptr() const { return reinterpret_cast<pointer>(m_buffer); }

private:
    std::byte* m_buffer;
};

} // namespace memory

// bring symbols into parent namespace

using memory::buffer_ptr;

} // namespace shard
