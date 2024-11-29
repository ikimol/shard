// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/meta/type_traits.hpp>

#include <cstring>
#include <memory>

namespace shard {
namespace containers {

/// Represents a sparse set of unsigned values
template <typename T>
class sparse_set {
    static_assert(std::is_constructible_v<T, std::size_t> || std::is_convertible_v<T, std::size_t>);

public:
    using value_type = unqualified_t<T>;
    using index_type = std::size_t;
    using size_type = std::size_t;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;

public:
    /// Default constructor
    sparse_set() = default;

    /// Copy constructor
    sparse_set(const sparse_set& other)
    : m_size(other.m_size)
    , m_capacity(other.m_capacity) {
        copy_from(other, m_capacity);
    }

    /// Move constructor
    sparse_set(sparse_set&& other) noexcept
    : m_dense(std::move(other.m_dense))
    , m_sparse(std::move(other.m_sparse))
    , m_size(other.m_size)
    , m_capacity(other.m_capacity) {
        other.m_size = 0;
        other.m_capacity = 0;
    }

    /// Copy assignment operator
    sparse_set& operator=(const sparse_set& other) {
        if (this != &other) {
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            copy_from(other, m_capacity);
        }
        return *this;
    }

    /// Move assignment operator
    sparse_set& operator=(sparse_set&& other) noexcept {
        if (this != &other) {
            m_dense = std::move(other.m_dense);
            m_sparse = std::move(other.m_sparse);
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            other.m_size = 0;
            other.m_capacity = 0;
        }
        return *this;
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

    iterator begin() { return m_dense.get(); }

    const_iterator begin() const { return m_dense.get(); }

    iterator end() { return m_dense.get() + m_size; }

    const_iterator end() const { return m_dense.get() + m_size; }

private:
    struct free_deleter {
        void operator()(void* ptr) const { std::free(ptr); }
    };

private:
    void copy_from(const sparse_set& other, std::size_t capacity) {
        void* dense;
        if (dense = std::malloc(capacity * sizeof(value_type)); !dense) {
            throw std::bad_alloc();
        }

        void* sparse;
        if (sparse = std::malloc(capacity * sizeof(index_type)); !sparse) {
            std::free(dense);
            throw std::bad_alloc();
        }

        std::memcpy(dense, other.m_dense.get(), capacity * sizeof(value_type));
        m_dense.reset(static_cast<value_type*>(dense));

        std::memcpy(sparse, other.m_sparse.get(), capacity * sizeof(index_type));
        m_sparse.reset(static_cast<index_type*>(sparse));
    }

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
        void* dense;
        if (dense = std::realloc(m_dense.get(), new_capacity * sizeof(value_type)); !dense) /* NOLINT */ {
            throw std::bad_alloc();
        }

        void* sparse;
        if (sparse = std::realloc(m_sparse.get(), new_capacity * sizeof(index_type)); !sparse) /* NOLINT */ {
            std::free(dense);
            throw std::bad_alloc();
        }

        m_dense.release(); // avoid freeing memory
        m_dense.reset(static_cast<value_type*>(dense));

        m_sparse.release(); // avoid freeing memory
        m_sparse.reset(static_cast<index_type*>(sparse));

        m_capacity = new_capacity;
    }

    static index_type to_unsigned(value_type value) { return static_cast<std::size_t>(value); }

private:
    std::unique_ptr<value_type[], free_deleter> m_dense;  // dense set of elements
    std::unique_ptr<index_type[], free_deleter> m_sparse; // map of elements to dense set indices

    size_type m_size = 0;
    size_type m_capacity = 0;
};

} // namespace containers

// bring symbols into parent namespace

using containers::sparse_set;

} // namespace shard
