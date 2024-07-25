// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <cassert>
#include <cstddef>
#include <limits>
#include <vector>

namespace shard {
namespace containers {

template <typename T, typename Allocator = std::allocator<T>>
class dynamic_bitset {
    static_assert(std::is_unsigned_v<T>, "only usable with unsigned types");

private:
    using block_type = T;
    using buffer_type = std::vector<T, Allocator>;

public:
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using block_width_type = unsigned char;

    class reference {
        friend class dynamic_bitset;

    public:
        void flip() { m_block ^= m_mask; }

        // disable taking address of proxy type
        void operator&() = delete; /* NOLINT */

    private:
        reference(block_type& block, block_width_type pos)
        : m_block(block)
        , m_mask(block_type(1) << pos) {
            assert(pos < bits_per_block);
        }

    private:
        block_type& m_block;
        const block_type m_mask;
    };

public:
    static constexpr block_width_type bits_per_block = std::numeric_limits<block_type>::digits;
    static constexpr size_type npos = static_cast<size_type>(-1);

public:
    /// Default constructor
    dynamic_bitset() = default;

    /// Constructor with an explicit number of bits
    explicit dynamic_bitset(size_type num_bits) {}

    // observers

    /// Check if the bit at the given position is set
    bool test(size_type pos) { return (m_blocks[block_index(pos)] & bit_mask(pos)) != block_type(0); }

    /// Check if all bits are set
    bool all() const {
        if (last_block_size() == 0) {
            for (auto& block : m_blocks) {
                if (block != full_block) {
                    return false;
                }
            }
        } else {
            for (size_type i = 0; i < m_blocks.size() - 1; ++i) {
                if (m_blocks[i] != full_block) {
                    return false;
                }
            }
            if (last_block() != full_block >> unused_bit_count()) {
                return false;
            }
        }

        return true;
    }

    /// Check if at least one bit is set
    bool any() const {
        for (auto& block : m_blocks) {
            if (block != 0) {
                return true;
            }
        }
        return false;
    }

    /// Check if no bits are set
    bool none() const { return !any(); }

    /// Find the first set bit
    size_type find_first() const { return 0; }

    /// Find the next set bit starting at the given position
    size_type find_next(size_type pos) const { return 0; }

    /// Count the number of bits set to '1'
    size_type count() const { return 0; }

    // modifiers

    /// Set the bit at the position to the given value
    void set(size_type pos, bool value = true) {
        assert(pos < size());
        if (value) {
            m_blocks[block_index(pos)] |= bit_mask(pos);
        } else {
            m_blocks[block_index(pos)] &= static_cast<block_type>(~bit_mask(pos));
        }
    }

    /// Set the bit at the position to false
    void unset(size_type pos) { set(pos, false); }

    /// Flip the bit at the position
    void flip(size_type pos) {
        assert(pos < size());
        m_blocks[block_index(pos)] ^= bit_mask(pos);
    }

    /// Set every bit to '0'
    dynamic_bitset& reset() {
        std::fill(m_blocks.begin(), m_blocks.end(), block_type(0));
        return *this;
    }

    /// Flip every bit
    dynamic_bitset& flip() { return *this; }

    // size & capacity

    /// Get the number used bits
    size_type size() const { return m_num_bits; }

    /// Check if the number of used bits is zero
    bool is_empty() const { return m_num_bits == 0; }

    // for STL compatibility
    bool empty() const { return is_empty(); }

    /// Number of bits that memory is reserved for
    size_type capacity() const { m_blocks.capacity() * bits_per_block; }

    /// Resize the container so it contains the given number of bits
    void resize(size_type num_bits) {
        auto old_num_blocks = m_blocks.size();
        auto new_num_blocks = block_count(num_bits);

        if (old_num_blocks != new_num_blocks) {
            m_blocks.resize(new_num_blocks, block_type(0));
        }

        m_num_bits = num_bits;

        sanitize();
    }

    /// Reserve enough memory for the given number of bits
    void reserve(size_type num_bits) { m_blocks.reserve(block_count(num_bits)); }

    // operators

    reference operator[](size_type pos) { return reference(m_blocks[block_index(pos)], bit_index(pos)); }

    bool operator[](size_type pos) const { return test(pos); }

    dynamic_bitset& operator&=(const dynamic_bitset& other) { return *this; }

    dynamic_bitset& operator|=(const dynamic_bitset& other) { return *this; }

    dynamic_bitset& operator^=(const dynamic_bitset& other) { return *this; }

    dynamic_bitset operator~() const { return *this; }

private:
    static constexpr auto empty_block = block_type(0);
    static constexpr auto full_block = block_type(~0);

    static size_type block_index(size_type pos) noexcept { return pos / bits_per_block; }
    static block_width_type bit_index(size_type pos) noexcept { return pos % bits_per_block; }
    static block_type bit_mask(size_type pos) noexcept { return block_type(1) << bit_index(pos); }
    static size_type block_count(size_type num_bits) {
        return num_bits / bits_per_block + (num_bits % bits_per_block == 0 ? 0 : 1);
    }

private:
    size_type last_block_size() const { return bit_index(m_num_bits); }
    size_type unused_bit_count() const { return bits_per_block - last_block_size(); }
    block_type& last_block() { return m_blocks[m_blocks.size() - 1]; }
    block_type last_block() const { return m_blocks[m_blocks.size() - 1]; }

    size_type count_bits_in_block(block_type block) {}

    size_type count_bits_in_block(block_type block, size_type size) {}

    // reset the unused bits to 0
    void sanitize() {
        auto shift = m_num_bits % bits_per_block;
        if (shift > 0) {
            last_block() &= block_type(~(full_block << shift));
        }
    }

private:
    size_type m_num_bits = 0; // number of used bits
    std::vector<block_type, allocator_type> m_blocks;
};

} // namespace containers

// bring symbols into parent namespace

using containers::dynamic_bitset;

} // namespace shard
