// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/memory/allocator.hpp"
#include "shard/memory/utils.hpp"

namespace shard {
namespace memory {

class linear_allocator : public allocator {
public:
    linear_allocator(void* data, std::size_t size) noexcept
    : allocator(size)
    , m_data(data)
    , m_next(data) {}

    void* allocate(std::size_t size, std::size_t align) override {
        assert(size != 0 && align != 0);

        auto padding = get_padding(m_next, align);

        // not enough memory
        if (m_used_memory + padding + size > m_size) {
            return nullptr;
        }

        // properly align the object
        auto aligned_address = as_uint(m_next) + padding;
        m_next = as_ptr(aligned_address + size);
        m_used_memory = as_uint(m_next) - as_uint(m_data);
        ++m_allocation_count;

        return as_ptr(aligned_address);
    }

    void deallocate(void* /* ptr */) override {
        // no-op, use 'rewind()' or 'clear()'
        assert(false);
    }

    /// Reset the next pointer to the provided one
    void rewind(void* p) {
        assert(m_data < p && p < m_next);
        m_next = p;
        m_used_memory = as_uint(m_next) - as_uint(m_data);
    }

    /// Reset the next pointer to the beginning
    void clear() {
        m_next = m_data;
        m_used_memory = 0;
        m_allocation_count = 0;
    }

    /// Get the first address managed by this allocator
    void* data() const { return m_data; }

    /// Get the address that will be used for the next allocation
    void* next() const { return m_next; }

private:
    void* m_data = nullptr;
    void* m_next = nullptr;
};

} // namespace memory

// bring symbols into parent namespace

using memory::linear_allocator;

} // namespace shard
