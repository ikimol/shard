// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/meta/type_traits.hpp"

#include <bitset>
#include <cassert>
#include <type_traits>

namespace shard {
namespace enums {

template <typename E, std::size_t N>
class enum_set {
private:
    using value_type = E;
    using underlying_type = std::underlying_type_t<E>;

public:
    class iterator {
    public: // std::iterator conformance
        using iterator_category = std::forward_iterator_tag;
        using value_type = E;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

    public:
        iterator(const enum_set& set, underlying_type value)
        : m_set(set)
        , m_value(value) {}

        value_type operator*() const { return static_cast<value_type>(m_value); }

        iterator& operator++() {
            m_value = m_set.next_bit(m_value + 1);
            return *this;
        }

        bool operator==(const iterator& other) const { return m_value == other.m_value; };
        bool operator!=(const iterator& other) const { return m_value != other.m_value; };

    private:
        const enum_set& m_set;
        underlying_type m_value;
    };

public:
    /// Default constructor
    constexpr enum_set() noexcept = default;

    /// Construct with a number of values
    template <typename... Args>
    constexpr /* implicit */ enum_set(Args&&... args); /* NOLINT */

    /// Construct using a set of values
    constexpr enum_set(std::initializer_list<E> il)
    : m_bits(to_index_set(il.begin(), il.end())) {}

    /// Check if the bit for the value is set
    bool test(value_type value) const { return m_bits.test(to_index(value)); }

    /// Check if all bits are set
    bool all() const noexcept { return m_bits.all(); }

    /// Check if any bit is set
    bool any() const noexcept { return m_bits.any(); }

    /// Check if no bits are set
    bool none() const noexcept { return m_bits.none(); }

    /// Get the number of set bits
    std::size_t count() const noexcept { return m_bits.count(); }

    /// Access a bit by the value
    constexpr bool operator[](value_type value) const { return m_bits[to_index(value)]; }

    /// Access a bit by the value
    auto operator[](value_type value) { return m_bits[to_index(value)]; }

    /// Set the bit for the value
    enum_set& set(value_type value, bool flag = true);

    /// Set every bit
    enum_set& set_all() noexcept;

    /// Unset the bit for the value
    enum_set& reset(value_type value);

    /// Unset every bit
    enum_set& clear() noexcept;

    /// Set / unset the bit for the value
    enum_set& toggle(value_type value);

    /// Set / unset every bit
    enum_set& toggle_all() noexcept;

    /// Get the union of the two sets
    enum_set union_with(const enum_set& other) const { return enum_set(m_bits | other.m_bits); }

    /// Get the intersection of the two sets
    enum_set intersection_with(const enum_set& other) const { return enum_set(m_bits & other.m_bits); }

    /// Get a set by subtracting the other
    enum_set subtracting(const enum_set& other) const { return enum_set(m_bits & ~other.m_bits); }

    /// Check if this set is a subset of the other
    bool is_subset_of(const enum_set& other) const { return union_with(other) == other; }

    iterator begin() const { return iterator(*this, next_bit(0)); }

    iterator end() const { return iterator(*this, N); }

    bool operator==(const enum_set<E, N>& other) const { return m_bits == other.m_bits; }

    bool operator!=(const enum_set<E, N>& other) const { return m_bits != other.m_bits; }

    bool operator==(E other) const { return m_bits.count() == 1 && m_bits.test(to_index(other)); }

private:
    constexpr explicit enum_set(std::bitset<N> bits)
    : m_bits(bits) {}

    static constexpr inline underlying_type to_index(value_type value) {
        auto index = static_cast<underlying_type>(value);
        assert(index < (underlying_type)(N));
        return index;
    }

    template <typename Iterator>
    static constexpr inline underlying_type to_index_set(Iterator begin, Iterator end) {
        underlying_type result = 0;
        for (auto it = begin; it != end; ++it) {
            result |= 1 << to_index(*it);
        }
        return result;
    }

    underlying_type next_bit(underlying_type after) const {
        while (after < (underlying_type)(N) && !m_bits.test(after)) {
            ++after;
        }
        return after;
    }

private:
    std::bitset<N> m_bits;
};

// implementation

template <typename E, std::size_t N>
template <typename... Args>
constexpr enum_set<E, N>::enum_set(Args&&... args) {
    static_assert(are_same<E, Args...>::value, "mismatched value types");
    E values[sizeof...(args)] = {std::forward<Args>(args)...};
    m_bits = to_index_set(std::begin(values), std::end(values));
}

template <typename E, std::size_t N>
enum_set<E, N>& enum_set<E, N>::set(value_type value, bool flag) {
    m_bits.set(to_index(value), flag);
    return *this;
}

template <typename E, std::size_t N>
enum_set<E, N>& enum_set<E, N>::set_all() noexcept {
    m_bits.set();
    return *this;
}

template <typename E, std::size_t N>
enum_set<E, N>& enum_set<E, N>::reset(value_type value) {
    m_bits.reset(to_index(value));
    return *this;
}

template <typename E, std::size_t N>
enum_set<E, N>& enum_set<E, N>::clear() noexcept {
    m_bits.reset();
    return *this;
}

template <typename E, std::size_t N>
enum_set<E, N>& enum_set<E, N>::toggle(value_type value) {
    m_bits.flip(to_index(value));
    return *this;
}

template <typename E, std::size_t N>
enum_set<E, N>& enum_set<E, N>::toggle_all() noexcept {
    m_bits.flip();
    return *this;
}

} // namespace enums

// bring symbols into parent namespace

using enums::enum_set;

} // namespace shard
