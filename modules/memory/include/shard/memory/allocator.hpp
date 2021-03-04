// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_MEMORY_ALLOCATOR_HPP
#define SHARD_MEMORY_ALLOCATOR_HPP

#include <shard/core/non_copyable.hpp>

#include <cassert>
#include <cstddef>

namespace shard {
namespace memory {

class allocator {
public:
    explicit allocator(std::size_t size) noexcept : m_size(size) {}

    virtual ~allocator() {
        assert(m_used_memory == 0);
        assert(m_allocation_count == 0);
    }

    /// Allocate raw memory respecting the alignment
    virtual void* allocate(std::size_t size, std::size_t align) = 0;

    /// Deallocate previously allocated memory
    virtual void deallocate(void* ptr) = 0;

    /// Return the size of the memory the allocator operates on
    std::size_t size() const noexcept { return m_size; }

    /// Return the total amount of allocated memory in bytes
    std::size_t used_memory() const noexcept { return m_used_memory; }

    /// Return the number of allocations
    std::size_t allocation_count() const noexcept { return m_allocation_count; }

protected:
    std::size_t m_size;
    std::size_t m_used_memory = 0;
    std::size_t m_allocation_count = 0;
};

// helper functions

template <typename T, typename... Args>
T* new_object(allocator& a, Args&&... args) {
    return new (a.allocate(sizeof(T), alignof(T))) T(std::forward<Args>(args)...);
}

template <typename T, std::enable_if_t<!std::is_trivially_destructible<T>::value>* = nullptr>
void delete_object(allocator& a, T* object) {
    object->~T();
    a.deallocate(object);
}

template <typename T, std::enable_if_t<std::is_trivially_destructible<T>::value>* = nullptr>
void delete_object(allocator& a, T* object) {
    a.deallocate(object);
}

} // namespace memory

// bring symbols into parent namespace

using memory::allocator;
using memory::delete_object;
using memory::new_object;

} // namespace shard

#endif // SHARD_MEMORY_ALLOCATOR_HPP
