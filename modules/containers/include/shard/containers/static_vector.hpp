// Copyright (c) 2022 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/utility/preprocessor.hpp>

#include <cassert>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace shard {

template <typename T, std::size_t Capacity>
class static_vector {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    /// Default constructor
    static_vector() = default;

    /// Destructor
    ~static_vector() { clear(); }

    // modifiers

    /// Add a new element at the end
    constexpr void push_back(const value_type& value) {
        assert(m_size < Capacity);
        new (&m_data[m_size]) T(value);
        ++m_size;
    }

    /// Add a new element at the end
    constexpr void push_back(value_type&& value) {
        assert(m_size < Capacity);
        new (&m_data[m_size]) T(std::move(value));
        ++m_size;
    }

    /// Create a new element in-place at the end
    template <typename... Args>
    void emplace_back(Args&&... args) {
        assert(m_size < Capacity);
        new (&m_data[m_size]) T(SHARD_FWD(args)...);
        ++m_size;
    }

    /// Remove the last element
    void pop_back() {
        assert(m_size > 0);
        reinterpret_cast<pointer>(&m_data[m_size - 1])->~T();
        --m_size;
    }

    /// Remove every element
    void clear() {
        for (std::size_t i = 0; i < m_size; ++i) {
            reinterpret_cast<pointer>(&m_data[i])->~T();
        }
        m_size = 0;
    }

    // element access

    /// Get the first element
    reference first() {
        if (m_size == 0) {
            throw std::out_of_range("shard::containers::static_vector::first()");
        }
        return operator[](0);
    }

    /// Get the first element
    const_reference first() const {
        if (m_size == 0) {
            throw std::out_of_range("shard::containers::static_vector::first()");
        }
        return operator[](0);
    }

    /// Get the last element
    reference last() {
        if (m_size == 0) {
            throw std::out_of_range("shard::containers::static_vector::last()");
        }
        return operator[](m_size - 1);
    }

    /// Get the last element
    const_reference last() const {
        if (m_size == 0) {
            throw std::out_of_range("shard::containers::static_vector::last()");
        }
        return operator[](m_size - 1);
    }

    /// Get the element at the given index
    ///
    /// \note Will check if the index is in range
    reference at(size_type index) {
        auto& const_this = std::as_const(*this);
        return const_cast<reference>(const_this.at(index));
    }

    /// Get the element at the given index
    ///
    /// \note Will check if the index is in range
    const_reference at(size_type index) const {
        if (index >= m_size) {
            throw std::out_of_range("shard::containers::static_vector::at()");
        }
        return operator[](index);
    }

    /// Get the element at the given index
    ///
    /// \note Will *NOT* check if the index is in range
    reference operator[](size_type index) { return *reinterpret_cast<pointer>(&m_data[index]); }

    /// Get the element at the given index
    ///
    /// \note Will *NOT* check if the index is in range
    const_reference operator[](size_type index) const { return *reinterpret_cast<const_pointer>(&m_data[index]); }

    /// Return a pointer to the underlying raw array
    pointer data() noexcept { return reinterpret_cast<pointer>(m_data); }

    /// Return a pointer to the underlying raw array
    const_pointer data() const noexcept { return reinterpret_cast<const_pointer>(m_data); }

    // size & capacity

    /// Get the number of elements in the array
    constexpr size_type size() const noexcept { return m_size; }

    /// Check if the array is empty
    constexpr bool is_empty() const noexcept { return m_size == 0; }

    // for STL compatibility
    constexpr bool empty() const noexcept { return is_empty(); }

    /// Get the number of elements memory is reserved for
    static constexpr size_type capacity() noexcept { return Capacity; }

    // iterators

    iterator begin() noexcept { return data(); }

    const_iterator begin() const noexcept { return data(); }

    const_iterator cbegin() const noexcept { return data(); }

    iterator end() noexcept { return data() + m_size; }

    const_iterator end() const noexcept { return data() + m_size; }

    const_iterator cend() const noexcept { return data() + m_size; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }

    const_reverse_iterator crbegin() const noexcept { return rbegin(); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

    const_reverse_iterator crend() const noexcept { return rend(); }

private:
    static constexpr auto value_size = sizeof(value_type);
    static constexpr auto value_align = alignof(value_type);
    using storage_type = typename std::aligned_storage<value_size, value_align>::type;

private:
    storage_type m_data[Capacity];
    size_type m_size = 0;
};

} // namespace shard
