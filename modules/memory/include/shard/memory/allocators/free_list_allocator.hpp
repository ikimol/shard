// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_MEMORY_FREE_LIST_ALLOCATOR_HPP
#define SHARD_MEMORY_FREE_LIST_ALLOCATOR_HPP

#include "shard/memory/allocator.hpp"
#include "shard/memory/utils.hpp"

namespace shard {
namespace memory {

class free_list_allocator : public allocator {
public:
    free_list_allocator(void* data, std::size_t size) : allocator(size), m_blocks(reinterpret_cast<block*>(data)) {
        assert(size > sizeof(block));

        // the first block is 'size' bytes big and there are no other blocks
        m_blocks->size = size;
        m_blocks->next = nullptr;
    }

    void* allocate(std::size_t size, std::size_t align) override {
        assert(size != 0 && align != 0);

        block* prev_free_block = nullptr;
        block* free_block = m_blocks;

        block* best_fit_prev = nullptr;
        block* best_fit = nullptr;
        std::size_t best_fit_padding = 0;
        std::size_t best_fit_total_size = 0;

        // try to find the best fit by looking at every free block and picking
        // the one with the smallest size that is big enough
        while (free_block) {
            // calculate padding needed to keep object correctly aligned
            auto padding = get_padding<allocation_header>(free_block, align);
            auto total_size = size + padding;

            // use this block if it's a perfect fit
            if (free_block->size == total_size) {
                best_fit_prev = prev_free_block;
                best_fit = free_block;
                best_fit_padding = padding;
                best_fit_total_size = total_size;
                break;
            }

            // switch if a better fit is found or if no fit was found yet
            if (free_block->size > total_size && (!best_fit || free_block->size < best_fit->size)) {
                best_fit_prev = prev_free_block;
                best_fit = free_block;
                best_fit_padding = padding;
                best_fit_total_size = total_size;
            }

            prev_free_block = free_block;
            free_block = free_block->next;
        }

        // if no space was found
        if (!best_fit) {
            return nullptr;
        }

        // if the remaining memory after the allocation would not be enough for
        // another allocation at a later time
        if (best_fit->size - best_fit_total_size <= header_size) {
            // return the extra bytes of memory instead of creating a new block
            // to reduce fragmentation
            best_fit_total_size = best_fit->size;

            // unlink the chosen block from the list
            if (best_fit_prev) {
                best_fit_prev->next = best_fit->next;
            } else {
                // if this was the first free block
                m_blocks = best_fit->next;
            }
        } else {
            // prevent new block from overwriting the block info
            assert(best_fit_total_size > sizeof(block));

            // else create a new block with the remaining memory
            auto new_block = reinterpret_cast<block*>(add(best_fit, best_fit_total_size));
            new_block->size = best_fit->size - best_fit_total_size;
            new_block->next = best_fit->next;

            // insert the new block
            if (best_fit_prev) {
                best_fit_prev->next = new_block;
            } else {
                // if this was the first free block
                m_blocks = new_block;
            }
        }

        // properly align the chosen block
        auto aligned_address = as_uint(best_fit) + best_fit_padding;

        // padding includes the size of the allocation header, hence it is
        // subtracted from the aligned address
        auto header = reinterpret_cast<allocation_header*>(aligned_address - header_size);
        header->size = best_fit_total_size;
        header->padding = best_fit_padding;

        // check that the alignments are ok (should always be the case)
        assert(is_aligned(header));
        assert(is_aligned(as_ptr(aligned_address), align));

        m_used_memory += best_fit_total_size;
        ++m_allocation_count;

        return as_ptr(aligned_address);
    }

    void deallocate(void* ptr) override {
        assert(ptr);

        // get the header from the start of the block
        auto header = reinterpret_cast<allocation_header*>(sub(ptr, header_size));

        // calculate the bounds of the block to be deallocated
        auto block_start = as_uint(ptr) - header->padding;
        auto block_size = header->size;
        auto block_end = block_start + block_size;

        block* prev_free_block = nullptr;
        block* free_block = m_blocks;

        // starting from the beginning try to find the first free block before
        // the block being deallocated
        while (free_block) {
            if (as_uint(free_block) >= block_end) {
                break;
            }
            prev_free_block = free_block;
            free_block = free_block->next;
        }

        // if no previous free block was found simply make the one being
        // deallocated the next free block
        if (!prev_free_block) {
            prev_free_block = reinterpret_cast<block*>(block_start);
            prev_free_block->size = block_size;
            prev_free_block->next = m_blocks;
            m_blocks = prev_free_block;
        } else if (as_uint(prev_free_block) + prev_free_block->size == block_start) {
            // if the chosen free block is right before the one being
            // deallocated simply increase the free block's size
            prev_free_block->size += block_size;
        } else {
            // if the found free block is not adjacent
            auto tmp = reinterpret_cast<block*>(block_start);
            tmp->size = block_size;
            tmp->next = prev_free_block->next;

            // add the block being deallocated after the found free block
            prev_free_block->next = tmp;
            prev_free_block = tmp;
        }

        // the free block must be there
        assert(prev_free_block);

        // merge adjacent free blocks
        if (as_uint(prev_free_block) + prev_free_block->size == as_uint(prev_free_block->next)) {
            prev_free_block->size += prev_free_block->next->size;
            prev_free_block->next = prev_free_block->next->next;
        }

        m_used_memory -= block_size;
        --m_allocation_count;
    }

private:
    struct allocation_header {
        std::size_t size;
        std::size_t padding;
    };

    struct block {
        std::size_t size;
        block* next;
    };

private:
    static constexpr auto header_size = sizeof(allocation_header);

private:
    block* m_blocks;
};

} // namespace memory

// bring symbols into parent namespace

using memory::free_list_allocator;

} // namespace shard

#endif // SHARD_MEMORY_FREE_LIST_ALLOCATOR_HPP
