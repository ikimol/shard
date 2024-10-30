// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

namespace shard {
namespace containers {
namespace detail {

template <typename T = void>
struct bit_diff {
    T operator()(const T& x, const T& y) const { return x & ~y; }
};

template <>
struct bit_diff<void> {
    template <typename T1, typename T2>
    auto operator()(const T1& x, const T2& y) const {
        return x & ~y;
    }
};

} // namespace detail

template <typename Block = std::uint32_t, typename Allocator = std::allocator<Block>>
class dynamic_bitset {
    static_assert(std::is_unsigned_v<Block>, "not an unsigned type");

    // equality operator
    template <typename B, typename A>
    friend bool operator==(const dynamic_bitset<B, A>&, const dynamic_bitset<B, A>&);

private:
    using block_type = Block;
    using buffer_type = std::vector<Block, Allocator>;

public:
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using block_width_type = std::uint8_t;

    class reference {
        friend class dynamic_bitset;

    public:
        /// Set the value of the referenced bit
        reference& operator=(bool value) {
            assign(value);
            return *this;
        }

        /// Assign the value to the referenced bit
        void assign(bool value) {
            if (value) {
                set();
            } else {
                reset();
            }
        }

        /// Set the value of the referenced bit to 1
        void set() { m_block |= m_mask; }

        /// Set the value of the referenced bit to 0
        void reset() { m_block &= block_type(~m_mask); }

        /// Flip the value of the referenced bit
        void flip() { m_block ^= m_mask; }

        /// Negation operator for the referenced bit
        bool operator~() const { return (m_block & m_mask) == zero_block; }

        /// Conversion operator for the referenced bit
        /* implicit */ operator bool() const /* NOLINT */ { return (m_block & m_mask) != zero_block; }

        // disable taking address of proxy type
        void operator&() = delete; /* NOLINT */

    private:
        reference(block_type& block, block_width_type index)
        : m_block(block)
        , m_mask(block_type(1) << index) {
            assert(index < bits_per_block);
        }

    private:
        block_type& m_block;
        const block_type m_mask;
    };

public:
    static constexpr block_width_type bits_per_block = std::numeric_limits<block_type>::digits;
    static constexpr size_type npos = std::numeric_limits<size_type>::max();

public:
    /// Default constructor
    dynamic_bitset() = default;

    /// Copy constructor
    dynamic_bitset(const dynamic_bitset& other) = default;

    /// Move constructor
    dynamic_bitset(dynamic_bitset&&) noexcept = default;

    /// Copy assignment operator
    dynamic_bitset& operator=(const dynamic_bitset&) = default;

    /// Move assignment operator
    dynamic_bitset& operator=(dynamic_bitset&&) noexcept = default;

    /// Constructor with an explicit number of bits with the given value
    explicit dynamic_bitset(size_type num_bits, std::uint64_t value = 0)
    : m_size(num_bits)
    , m_blocks(blocks_required(num_bits), zero_block) {
        if (num_bits == 0 || value == 0) {
            return;
        }

        constexpr size_type bit_count = std::numeric_limits<std::uint64_t>::digits;
        constexpr auto required_blocks = bit_count / bits_per_block;

        if (required_blocks == 1) {
            m_blocks[0] = value;
        } else {
            auto mask = static_cast<std::uint64_t>(one_block);
            auto blocks_to_init = std::min(m_blocks.size(), required_blocks);
            for (auto i = 0ul; i < blocks_to_init; ++i) {
                m_blocks[i] = block_type((value >> (i * bits_per_block)) & mask);
            }
        }

        sanitize();
    }

    // size & capacity

    /// Get the number used bits
    size_type size() const { return m_size; }

    /// Get the number of blocks allocated
    size_type num_blocks() const { return m_blocks.size(); }

    /// Check if the number of used bits is zero
    bool empty() const { return m_size == 0; }

    /// Number of bits that memory is reserved for
    size_type capacity() const { return m_blocks.capacity() * bits_per_block; }

    /// Reserve enough memory for the given number of bits
    void reserve(size_type new_capacity) { m_blocks.reserve(blocks_required(new_capacity)); }

    /// Reduce size to match capacity
    void shrink_to_fit() { m_blocks.shrink_to_fit(); }

    // observers

    /// Check if the bit at the given position is set
    bool test(size_type index) const {
        assert(index < m_size);
        return (m_blocks[block_index(index)] & bit_mask(index)) != block_type(0);
    }

    /// Update the bit at the given position and return its previous value
    bool test_set(size_type index, bool value = true) {
        auto result = test(index);
        if (result != value) {
            set(index, value);
        }
        return result;
    }

    /// Check if all bits are set
    bool all() const {
        // full blocks
        for (auto i = 0ul; i < m_blocks.size() - 1; ++i) {
            if (m_blocks[i] != one_block) {
                return false;
            }
        }

        // last block
        if (last_block() != (one_block >> unused_bit_count())) {
            return false;
        }

        return true;
    }

    /// Check if at least one bit is set
    bool any() const {
        for (auto& block : m_blocks) {
            if (block != zero_block) {
                return true;
            }
        }
        return false;
    }

    /// Check if no bits are set
    bool none() const { return !any(); }

    /// Count the number of bits set to '1'
    size_type count() const {
        size_type count = 0;

        // full blocks
        for (auto i = 0ul; i < m_blocks.size() - 1; ++i) {
            count += count_bits_in_block(m_blocks[i]);
        }

        // last block
        if (extra_bit_count() == 0) {
            count += count_bits_in_block(last_block());
        } else {
            count += count_bits_in_block(last_block(), extra_bit_count());
        }

        return count;
    }

    // modifiers

    /// Add a new bit with the given value
    void push_back(bool value) {
        auto bit_to_set = m_size++;

        if (m_size <= m_blocks.size() * bits_per_block) {
            if (value) {
                set(bit_to_set, value);
            }
        } else {
            m_blocks.push_back(block_type(value));
        }

        assert(check_consistency());
    }

    /// Remove the bit at the end
    void pop_back() {
        assert(!empty());

        --m_size;
        if (m_blocks.size() > blocks_required(m_size)) {
            m_blocks.pop_back();
        } else {
            sanitize();
        }

        assert(check_consistency());
    }

    /// Resize the container so it contains the given number of bits
    void resize(size_type new_size, bool value = false) {
        if (new_size == m_size) {
            return;
        }

        auto old_block_count = m_blocks.size();
        auto new_block_count = blocks_required(new_size);
        auto block_value = value ? one_block : zero_block;

        if (new_block_count != old_block_count) {
            m_blocks.resize(new_block_count, block_value);
        }

        if (value && new_size > m_size && old_block_count > 0) {
            if (extra_bit_count() > 0) {
                last_block() |= block_type(one_block << extra_bit_count());
            }
        }

        m_size = new_size;
        sanitize();
        assert(check_consistency());
    }

    /// Clear the bits and reset the size
    void clear() {
        m_size = 0;
        m_blocks.clear();
    }

    /// Set the bit at the position to the given value
    dynamic_bitset& set(size_type index, bool value = true) {
        assert(index < size());
        if (value) {
            m_blocks[block_index(index)] |= bit_mask(index);
        } else {
            m_blocks[block_index(index)] &= static_cast<block_type>(~bit_mask(index));
        }
        return *this;
    }

    /// Set every bit to 1
    dynamic_bitset& set() {
        std::fill(m_blocks.begin(), m_blocks.end(), one_block);
        sanitize();
        return *this;
    }

    /// Set the bit at the position to false
    dynamic_bitset& reset(size_type index) { return set(index, false); }

    /// Set every bit to 0
    dynamic_bitset& reset() {
        std::fill(m_blocks.begin(), m_blocks.end(), zero_block);
        return *this;
    }

    /// Flip the bit at the position
    dynamic_bitset& flip(size_type index) {
        assert(index < size());
        m_blocks[block_index(index)] ^= bit_mask(index);
        return *this;
    }

    /// Flip every bit
    dynamic_bitset& flip() {
        std::transform(m_blocks.cbegin(), m_blocks.cend(), m_blocks.begin(), std::bit_not {});
        sanitize();
        return *this;
    }

    // iteration

    /// Find the first set bit
    size_type find_first() const {
        for (auto i = 0ul; i < m_blocks.size(); ++i) {
            if (m_blocks[i] != zero_block) {
                return i * bits_per_block + count_trailing_zero_bits_in_block(m_blocks[i]);
            }
        }
        return npos;
    }

    /// Find the next set bit starting at the given position
    size_type find_next(size_type start_index) const {
        if (empty() || start_index >= size() - 1) {
            return npos;
        }

        auto first_bit = start_index + 1;
        auto first_block = block_index(first_bit);
        auto first_bit_index = bit_index(first_bit);
        auto first_block_shifted = block_type(m_blocks[first_block] >> first_bit_index);

        if (first_block_shifted != zero_block) {
            return first_bit + count_trailing_zero_bits_in_block(first_block_shifted);
        }

        for (auto i = first_block + 1; i < m_blocks.size(); ++i) {
            if (m_blocks[i] != zero_block) {
                return i * bits_per_block + count_trailing_zero_bits_in_block(m_blocks[i]);
            }
        }

        return npos;
    }

    // operators

    /// Get a reference to the bit at the given index
    reference operator[](size_type index) { return reference(m_blocks[block_index(index)], bit_index(index)); }

    /// Get the value of the bit at the given index
    bool operator[](size_type index) const { return test(index); }

    dynamic_bitset& operator&=(const dynamic_bitset& other) {
        assert(size() == other.size());
        std::transform(m_blocks.cbegin(), m_blocks.cend(), other.m_blocks.cbegin(), m_blocks.begin(), std::bit_and {});
        return *this;
    }

    dynamic_bitset& operator|=(const dynamic_bitset& other) {
        assert(size() == other.size());
        std::transform(m_blocks.cbegin(), m_blocks.cend(), other.m_blocks.cbegin(), m_blocks.begin(), std::bit_or {});
        return *this;
    }

    dynamic_bitset& operator^=(const dynamic_bitset& other) {
        assert(size() == other.size());
        std::transform(m_blocks.cbegin(), m_blocks.cend(), other.m_blocks.cbegin(), m_blocks.begin(), std::bit_xor {});
        return *this;
    }

    dynamic_bitset& operator-=(const dynamic_bitset& other) {
        assert(size() == other.size());
        std::transform(
            m_blocks.cbegin(), m_blocks.cend(), other.m_blocks.cbegin(), m_blocks.begin(), detail::bit_diff {});
        return *this;
    }

    dynamic_bitset operator~() const {
        dynamic_bitset copy(*this);
        copy.flip();
        return copy;
    }

    // conversion

    template <typename Char = char,
              typename CharTraits = std::char_traits<Char>,
              typename CharAllocator = std::allocator<Char>>
    std::basic_string<Char, CharTraits, CharAllocator> to_string(Char zero = Char('0'), Char one = Char('1')) const {
        std::basic_string<Char, CharTraits, CharAllocator> result(m_size, zero);
        for (auto i_block = 0ul; i_block < m_blocks.size(); ++i_block) {
            if (m_blocks[i_block] == zero_block) {
                continue;
            }

            block_type mask = 1;
            for (auto i_bit = 0ul; i_bit < bits_per_block; ++i_bit) {
                if ((m_blocks[i_block] & mask) != zero_block) {
                    CharTraits::assign(result[m_size - (i_block * bits_per_block + i_bit) - 1], one);
                }
                mask = block_type(mask << 1);
            }
        }
        return result;
    }

    // utility

    /// Swap two bitsets
    void swap(dynamic_bitset& other) noexcept {
        using std::swap;
        swap(m_blocks, other.m_blocks);
        swap(m_size, other.m_size);
    }

private:
    static constexpr auto zero_block = block_type(0);
    static constexpr auto one_block = block_type(~zero_block);

    static size_type block_index(size_type index) noexcept { return index / bits_per_block; }
    static block_width_type bit_index(size_type index) noexcept { return index % bits_per_block; }
    static block_type bit_mask(size_type index) noexcept { return block_type(1) << bit_index(index); }
    static size_type blocks_required(size_type size) noexcept { return (size + bits_per_block - 1) / bits_per_block; }

private:
    block_type& last_block() { return m_blocks[m_blocks.size() - 1]; }
    block_type last_block() const { return m_blocks[m_blocks.size() - 1]; }

    // used & unused bits in the last block
    size_type extra_bit_count() const { return bit_index(m_size); }
    size_type unused_bit_count() const { return bits_per_block - extra_bit_count(); }

    static size_type count_bits_in_block(block_type block) noexcept {
        return count_bits_in_block(block, bits_per_block);
    }

    static size_type count_bits_in_block(block_type block, size_type bit_count) noexcept {
        size_type count = 0;
        block_type mask = 1;
        for (auto i = 0ul; i < bit_count; ++i) {
            count += (block & mask) != zero_block ? 1 : 0;
            mask = block_type(mask << 1);
        }
        return count;
    }

    static size_type count_trailing_zero_bits_in_block(block_type block) noexcept {
        block_type mask = 1;
        for (auto i = 0ul; i < bits_per_block; ++i) {
            if ((block & mask) != zero_block) {
                return i;
            }
            mask = block_type(mask << 1);
        }
        return npos;
    }

    // reset unused bits to 0
    void sanitize() {
        auto shift = m_size % bits_per_block;
        if (shift > 0) {
            last_block() &= static_cast<block_type>(~(one_block << shift));
        }
    }

    bool check_unused_bits() const noexcept {
        if (extra_bit_count() > 0) {
            return (last_block() & (one_block << extra_bit_count())) == zero_block;
        }
        return true;
    }

    bool check_size() const noexcept { return blocks_required(size()) == m_blocks.size(); }
    bool check_consistency() const noexcept { return check_unused_bits() && check_size(); }

private:
    size_type m_size = 0; // number of used bits
    std::vector<block_type, allocator_type> m_blocks;
};

// operators

template <typename Block, typename Allocator>
bool operator==(const dynamic_bitset<Block, Allocator>& lhs, const dynamic_bitset<Block, Allocator>& rhs) {
    return lhs.m_size == rhs.m_size && lhs.m_blocks == rhs.m_blocks;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator> operator&(const dynamic_bitset<Block, Allocator>& lhs,
                                           const dynamic_bitset<Block, Allocator>& rhs) {
    dynamic_bitset<Block, Allocator> result(lhs);
    return result &= rhs;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator> operator|(const dynamic_bitset<Block, Allocator>& lhs,
                                           const dynamic_bitset<Block, Allocator>& rhs) {
    dynamic_bitset<Block, Allocator> result(lhs);
    return result |= rhs;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator> operator^(const dynamic_bitset<Block, Allocator>& lhs,
                                           const dynamic_bitset<Block, Allocator>& rhs) {
    dynamic_bitset<Block, Allocator> result(lhs);
    return result ^= rhs;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator> operator-(const dynamic_bitset<Block, Allocator>& lhs,
                                           const dynamic_bitset<Block, Allocator>& rhs) {
    dynamic_bitset<Block, Allocator> result(lhs);
    return result -= rhs;
}

} // namespace containers

// bring symbols into parent namespace

using containers::dynamic_bitset;

} // namespace shard
