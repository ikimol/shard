// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/meta/type_traits.hpp>

namespace shard {
namespace containers {

template <typename T>
struct sparse_set_value_translator {
    template <typename U = T, std::enable_if_t<std::is_unsigned_v<U>>* = nullptr>
    static std::size_t translate(U value) {
        return static_cast<std::size_t>(value);
    }
};

/// Represents a sparse set of unsigned values
template <typename T>
class sparse_set {
public:
    using value_type = unqualified_t<T>;
    using index_type = std::size_t;
    using size_type = std::size_t;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using translator_type = sparse_set_value_translator<value_type>;

public:
    ~sparse_set() {
        std::free(m_dense);
        std::free(m_sparse);
    }

    /// Add a new value to the set
    void insert(value_type value) {
        if (!contains(value)) {
            auto u_value = to_unsigned(value);
            ensure_element_fits(u_value);
            m_dense[m_size] = value;
            m_sparse[u_value] = m_size;
            ++m_size;
        }
    }

    /// Remove a value from the set
    void erase(value_type value) {
        if (contains(value)) {
            auto u_value = to_unsigned(value);
            m_dense[m_sparse[u_value]] = m_dense[m_size - 1];
            m_sparse[to_unsigned(m_dense[m_size - 1])] = m_sparse[u_value];
            --m_size;
        }
    }

    /// Empty the set
    void clear() { m_size = 0; }

    /// Check if the value is present in the set
    bool contains(value_type value) const {
        auto u_value = to_unsigned(value);
        return u_value < m_capacity && m_sparse[u_value] < m_size && to_unsigned(m_dense[m_sparse[u_value]]) == u_value;
    }

    /// Get the index of the value in the dense set
    index_type index_of(value_type value) const { return m_sparse[to_unsigned(value)]; }

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
    void ensure_element_fits(index_type index) {
        if (index + 1 > m_capacity) {
            grow(index + 1);
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
        if (new_capacity == m_capacity) {
            return;
        }

        void* dense;
        if (dense = std::realloc(m_dense, new_capacity * sizeof(value_type)); !dense) {
            throw std::bad_alloc();
        }

        void* sparse;
        if (sparse = std::realloc(m_sparse, new_capacity * sizeof(index_type)); !sparse) {
            std::free(dense);
            throw std::bad_alloc();
        }

        m_dense = static_cast<value_type*>(dense);
        m_sparse = static_cast<index_type*>(sparse);

        m_capacity = new_capacity;
    }

    static index_type to_unsigned(value_type value) { return translator_type::translate(value); }

private:
    value_type* m_dense = nullptr;  // dense set of elements
    index_type* m_sparse = nullptr; // map of elements to dense set indices

    size_type m_size = 0;
    size_type m_capacity = 0;
};

} // namespace containers

// bring symbols into parent namespace

using containers::sparse_set;
using containers::sparse_set_value_translator;

} // namespace shard
