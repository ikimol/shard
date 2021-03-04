// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_MEMORY_PROXY_ALLOCATOR_HPP
#define SHARD_MEMORY_PROXY_ALLOCATOR_HPP

#include "shard/memory/allocator.hpp"

namespace shard {
namespace memory {

class proxy_allocator : public allocator {
public:
    explicit proxy_allocator(allocator& a, const char* name = "") : allocator(a.size()), m_allocator(a), m_name(name) {}

    void* allocate(std::size_t size, std::size_t align) override {
        assert(size != 0);
        ++m_allocation_count;
        auto used = m_allocator.used_memory();
        auto ptr = m_allocator.allocate(size, align);
        // calculate the actual size of the allocation, because an allocation
        // might allocate more memory than requested
        m_used_memory += m_allocator.used_memory() - used;
        return ptr;
    }

    void deallocate(void* ptr) override {
        assert(ptr);
        --m_allocation_count;
        auto used = m_allocator.used_memory();
        m_allocator.deallocate(ptr);
        m_used_memory -= used - m_allocator.used_memory();
    }

    const char* name() const { return m_name; }

private:
    allocator& m_allocator;
    const char* m_name = nullptr;
};

} // namespace memory

// bring symbols into parent namespace

using memory::proxy_allocator;

} // namespace shard

#endif // SHARD_MEMORY_PROXY_ALLOCATOR_HPP
