// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <vector>

namespace shard {
namespace containers {

/// Represents a sparse set of unsigned values
class sparse_set {
public:
    using value_type = std::size_t;
    using size_type = std::size_t;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;

public:
    ~sparse_set() {
        std::free(m_dense);
        std::free(m_sparse);
    }

    /// Add a new value to the set
    void insert(value_type value) {
        if (!contains(value)) {
            ensure_element_fits(value);
            m_dense[m_size] = value;
            m_sparse[value] = m_size;
            ++m_size;
        }
    }

    /// Remove a value from the set
    void erase(value_type value) {
        if (contains(value)) {
            m_dense[m_sparse[value]] = m_dense[m_size - 1];
            m_sparse[m_dense[m_size - 1]] = m_sparse[value];
            --m_size;
        }
    }

    /// Empty the set
    void clear() { m_size = 0; }

    /// Check if the value is present in the set
    bool contains(value_type value) {
        return value < m_capacity && m_sparse[value] < m_size && m_dense[m_sparse[value]] == value;
    }

    /// Check if the set is empty
    bool is_empty() const noexcept { return m_size == 0; }

    /// Get the number of elements in the set
    size_type size() const { return m_size; }

    /// Get the number of elements memory is reserved for
    size_type capacity() const { return m_capacity; }

    /// Reserve memory if (needed) for more elements
    void reserve(size_type new_capacity) {
        if (new_capacity > m_capacity) {
            reallocate(new_capacity);
        }
    }

    // iterators

    iterator begin() { return m_dense; }

    const_iterator begin() const { return m_dense; }

    iterator end() { return m_dense + m_size; }

    const_iterator end() const { return m_dense + m_size; }

private:
    void ensure_element_fits(value_type value) {
        if (value + 1 > m_capacity) {
            grow(value + 1);
        }
    }

    void grow(size_type min_capacity) {
        size_type new_capacity = 4;
        while (new_capacity < min_capacity) {
            new_capacity *= 2;
        }
        reallocate(new_capacity);
    }

    void reallocate(size_type new_capacity) {
        if (new_capacity <= m_capacity) {
            return;
        }

        void* dense;
        if (dense = std::realloc(m_dense, new_capacity); !dense) {
            throw std::bad_alloc();
        }

        void* sparse;
        if (sparse = std::realloc(m_sparse, new_capacity); !sparse) {
            std::free(dense);
            throw std::bad_alloc();
        }

        m_dense = static_cast<value_type*>(dense);
        m_sparse = static_cast<value_type*>(sparse);

        m_capacity = new_capacity;
    }

private:
    value_type* m_dense = nullptr;  // dense set of elements
    value_type* m_sparse = nullptr; // map of elements to dense set indices

    size_type m_size = 0;
    size_type m_capacity = 0;
};

} // namespace containers

// bring symbols into parent namespace

using containers::sparse_set;

} // namespace shard
