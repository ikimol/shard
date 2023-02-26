// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/memory/allocator.hpp"
#include "shard/memory/utils.hpp"

namespace shard {
namespace memory {

class pool_allocator_base : public allocator {
public:
    pool_allocator_base(void* start, std::size_t size, std::size_t obj_size, std::size_t obj_align)
    : allocator(size)
    , m_obj_size(obj_size)
    , m_obj_align(obj_align) {
        assert(obj_size >= sizeof(void*));

        auto padding = get_padding(start, obj_align);

        // properly align the start of the free list
        // 'm_free_list' is a pointer to an array of 'obj_size' blocks of memory
        m_free_list = reinterpret_cast<void**>(add(start, padding));

        auto count = (size - padding) / obj_size;
        void** p = m_free_list;

        // initialize the list of objects
        for (std::size_t i = 0; i < count - 1; ++i) {
            // move the array forward by 1 step ('obj_size' bytes)
            *p = add(p, obj_size);
            p = reinterpret_cast<void**>(*p);
        }

        // the last element of the array points to null indicating there is no
        // more free chunks
        *p = nullptr;
    }

    void* allocate(std::size_t size, std::size_t align) override {
        assert(size == m_obj_size && align == m_obj_align);

        if (!m_free_list) {
            return nullptr;
        }

        // take the first chunk from the free list
        auto ptr = m_free_list;

        // make the next free chunk the head of the free list
        // (*m_free_list is the pointer to the next free chunk)
        m_free_list = reinterpret_cast<void**>(*m_free_list);
        m_used_memory += size;
        ++m_allocation_count;

        return ptr;
    }

    void deallocate(void* ptr) override {
        assert(ptr);
        // make the pointer being deallocated point to the free list
        *reinterpret_cast<void**>(ptr) = m_free_list;
        // add the pointer to the beginning of the free list
        m_free_list = reinterpret_cast<void**>(ptr);
        m_used_memory -= m_obj_size;
        --m_allocation_count;
    }

private:
    std::size_t m_obj_size;
    std::size_t m_obj_align;
    void** m_free_list = nullptr;
};

template <typename T>
class pool_allocator : public pool_allocator_base {
public:
    pool_allocator(void* start, std::size_t size)
    : pool_allocator_base(start, size, sizeof(T), alignof(T)) {}
};

} // namespace memory

// bring symbols into parent namespace

using memory::pool_allocator;

} // namespace shard
