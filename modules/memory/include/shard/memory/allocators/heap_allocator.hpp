// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/memory/allocator.hpp"
#include "shard/memory/utils.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

namespace shard {
namespace memory {

class heap_allocator : public allocator {
public:
    heap_allocator()
    : allocator(0) {}

    void* allocate(std::size_t size, std::size_t align) override {
        assert(size != 0);

        auto total_size = size + header_size;
        auto ptr = std::malloc(total_size);

        // check if allocation was successful
        if (!ptr) {
            return nullptr;
        }

        // clear the bytes in the allocated memory block
        std::memset(ptr, '\0', total_size);

        // padding includes the size of the allocation header, hence it is
        // subtracted from the aligned address
        auto header = reinterpret_cast<allocation_header*>(ptr);
        header->size = total_size;

        auto aligned_address = add(ptr, header_size);

        // check that the alignments are ok (should always be the case)
        assert(is_aligned(header));
        assert(is_aligned(aligned_address, align));

        m_used_memory += total_size;
        ++m_allocation_count;

        return aligned_address;
    }

    void deallocate(void* ptr) override {
        assert(ptr);

        // get the header at the start of the allocated memory
        auto header = reinterpret_cast<allocation_header*>(sub(ptr, header_size));

        m_used_memory -= header->size;
        --m_allocation_count;

        // return the memory
        std::free(header);
    }

private:
    struct allocation_header {
        std::size_t size;
    };

private:
    static constexpr auto header_size = sizeof(allocation_header);
};

} // namespace memory

// bring symbols into parent namespace

using memory::heap_allocator;

} // namespace shard
