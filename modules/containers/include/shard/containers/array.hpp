// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_CONTAINERS_ARRAY_HPP
#define SHARD_CONTAINERS_ARRAY_HPP

#include <shard/memory/allocator.hpp>

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace shard {
namespace containers {

template <typename T>
class array {
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
    explicit array(allocator& a, size_type capacity = 0) : m_allocator(&a), m_capacity(capacity) {
        if (m_capacity > 0) {
            m_data = allocate(capacity);
        }
    }

    template <typename Iterator>
    array(allocator& a, Iterator begin, Iterator end) : m_allocator(&a), m_capacity(0) {
        auto size = std::distance(begin, end);
        if (size > 0) {
            m_data = allocate(size);
            m_size = size;
            m_capacity = size;
            std::copy(begin, end, m_data);
        }
    }

    array(allocator& a, std::initializer_list<value_type> il) : m_allocator(&a) {
        if (il.size() > 0) {
            m_data = allocate(il.size());
            m_size = il.size();
            m_capacity = il.size();
            auto p = m_data;
            for (auto it = il.begin(); it != il.end(); ++it, ++p) {
                *p = *it;
            }
        } else {
            m_data = nullptr;
            m_size = 0;
            m_capacity = 0;
        }
    }

    /// Copy constructor
    array(const array& other) : m_allocator(other.m_allocator), m_size(other.m_size), m_capacity(other.m_capacity) {
        if (m_capacity > 0) {
            auto data = allocate(m_capacity);
            m_data = static_cast<pointer>(std::memcpy(data, other.m_data, m_capacity * value_size));
        }
    }

    /// Move constructor
    array(array&& other) noexcept :
    m_allocator(other.m_allocator),
    m_data(other.m_data),
    m_size(other.m_size),
    m_capacity(other.m_capacity) {
        // invalidate "moved-from" object
        other.m_allocator = nullptr;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    ~array() { deallocate(); }

    /// Copy assignment operator
    array& operator=(const array& other) {
        if (this != &other) {
            // destroy elements and deallocate memory
            deallocate();

            // allocate new memory
            m_allocator = other.m_allocator;
            if (other.m_capacity > 0) {
                auto data = allocate(other.m_capacity);
                m_data = static_cast<pointer>(std::memcpy(data, other.m_data, other.m_capacity * value_size));
            }
            m_size = other.m_size;
            m_capacity = other.m_capacity;
        }
        return *this;
    }

    /// Move assignment operator
    array& operator=(array&& other) noexcept {
        // destroy elements and deallocate memory
        deallocate();

        // allocate new memory
        m_allocator = other.m_allocator;
        if (other.m_capacity > 0) {
            m_data = allocate(other.m_capacity);
            for (std::size_t i = 0; i < other.m_size; ++i) {
                m_data[i] = std::move(other.m_data[i]);
            }
        }
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        // deallocate "moved-from" memory
        other.m_allocator->deallocate(other.m_data);

        // invalidate "moved-from" object
        other.m_allocator = nullptr;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;

        return *this;
    }

    // modifiers

    /// Add a new element at the end
    void append(const_reference value) {
        ensure_element_fits();
        m_data[m_size++] = value;
    }

    /// Add a new element at the end
    void append(value_type&& value) {
        ensure_element_fits();
        m_data[m_size++] = std::move(value);
    }

    // for STL compatibility
    void push_back(const_reference value) { append(value); }

    // for STL compatibility
    void push_back(value_type&& value) { append(std::move(value)); }

    /// Add a new element at the specified position
    iterator insert(const_iterator pos, const_reference value) {
        ensure_element_fits();
        auto i = pos ? pos - cbegin() : 0;
        auto p = m_data + i;
        // move the elements in the range [p, end) to start at 'p + 1'
        std::move_backward(p, m_data + m_size, m_data + m_size + 1);
        m_data[i] = value;
        ++m_size;
        return m_data + i;
    }

    /// Add a new element at the specified position
    iterator insert(const_iterator pos, value_type&& value) {
        ensure_element_fits();
        auto i = pos ? pos - cbegin() : 0;
        auto p = m_data + i;
        // move the elements in the range [p, end) to start at 'p + 1'
        std::move_backward(p, m_data + m_size, m_data + m_size + 1);
        m_data[i] = std::move(value);
        ++m_size;
        return m_data + i;
    }

    /// Create a new element in-place at the specified position
    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        ensure_element_fits();
        auto i = pos ? pos - cbegin() : 0;
        auto p = m_data + i;
        // move the elements in the range [p, end) to start at 'p + 1'
        std::move_backward(p, m_data + m_size, m_data + m_size + 1);
        new (m_data + i) value_type(std::forward<Args>(args)...);
        ++m_size;
        return m_data + i;
    }

    /// Create a new element in-place at the end
    template <typename... Args>
    void emplace_back(Args&&... args) {
        ensure_element_fits();
        new (m_data + m_size) value_type(std::forward<Args>(args)...);
        ++m_size;
    }

    /// Remove the element at the specified position
    iterator remove(const_iterator pos) {
        assert(pos != end());
        auto p = m_data + (pos - cbegin());
        // move the elements in the range [p + 1, end) to start at 'p'
        destruct_after(std::move(p + 1, m_data + m_size, p));
        return p;
    }

    /// Remove the elements in the specified range
    iterator remove(const_iterator first, const_iterator last) {
        assert(first <= last);
        auto start = first - cbegin();
        auto p_first = m_data + start;
        if (first != last) {
            auto p_last = p_first + (last - first);
            // move the elements in the range [last, end) to start at 'p_first'
            // note: this does not move anything if last == end
            destruct_after(std::move(p_last, m_data + m_size, p_first));
        }
        return p_first;
    }

    /// Remove the element at the end
    void remove_last() {
        assert(m_size > 0);
        destruct_after(m_data + m_size - 1);
    }

    // for STL compatibility
    void pop_back() { remove_last(); }

    /// Remove every element
    ///
    /// \note: This does *NOT* deallocate the used memory
    void clear() {
        if (!m_data) {
            return;
        }

        // destroy all the elements
        destruct_after(m_data);

        // reset the information
        m_size = 0;
    }

    /// Exchange the contents of the array with those of another
    void swap(array& other) {
        using std::swap;
        swap(m_allocator, other.m_allocator);
        swap(m_data, other.m_data);
        swap(m_size, other.m_size);
        swap(m_capacity, other.m_capacity);
    }

    // element access

    /// Get the first element
    reference first() {
        if (m_size == 0) {
            throw std::out_of_range("shard::containers::array::first()");
        }
        return m_data[0];
    }

    /// Get the first element
    const_reference first() const {
        if (m_size == 0) {
            throw std::out_of_range("shard::containers::array::first()");
        }
        return m_data[0];
    }

    /// Get the last element
    reference last() {
        if (m_size == 0) {
            throw std::out_of_range("shard::containers::array::last()");
        }
        return m_data[m_size - 1];
    }

    /// Get the last element
    const_reference last() const {
        if (m_size == 0) {
            throw std::out_of_range("shard::containers::array::last()");
        }
        return m_data[m_size - 1];
    }

    /// Get the element at the given index
    ///
    /// \note Will check if the index is in range
    reference at(size_type index) {
        auto& const_this = static_cast<const array&>(*this);
        return const_cast<reference>(const_this.at(index));
    }

    /// Get the element at the given index
    ///
    /// \note Will check if the index is in range
    const_reference at(size_type index) const {
        if (index >= m_size) {
            throw std::out_of_range("shard::containers::array::at()");
        }
        return m_data[index];
    }

    /// Get the element at the given index
    ///
    /// \note Will *NOT* check if the index is in range
    reference operator[](size_type index) { return m_data[index]; }

    /// Get the element at the given index
    ///
    /// \note Will *NOT* check if the index is in range
    const_reference operator[](size_type index) const { return m_data[index]; }

    /// Return a pointer to the underlying raw array
    pointer data() noexcept { return m_data; }

    /// Return a pointer to the underlying raw array
    const_pointer data() const noexcept { return m_data; }

    // size & capacity

    /// Get the number of elements in the array
    size_type size() const noexcept { return m_size; }

    /// Check if the array is empty
    bool is_empty() const noexcept { return m_size == 0; }

    // for STL compatibility
    bool empty() const noexcept { return is_empty(); }

    /// Get the number of elements memory is reserved for
    size_type capacity() const noexcept { return m_capacity; }

    /// Reserve memory if (needed) for more elements
    void reserve(size_type new_capacity) {
        if (new_capacity > m_capacity) {
            reallocate(new_capacity);
        }
    }

    /// Set the size and potentially incur a reallocation
    ///
    /// \note This will not reduce memory usage even if the new size is less
    /// than the current size
    void resize(size_type new_size) {
        if (new_size > m_capacity) {
            grow(new_size);
        }
        if (new_size < m_size) {
            // destroy elements
            destruct_after(m_data + new_size);
        } else if (new_size > m_size) {
            // default construct elements in-place
            for (auto i = m_size; i < new_size; ++i) {
                new (m_data + i) value_type();
            }
        }
        m_size = new_size;
    }

    /// Reallocate exactly the amount of memory needed to store the elements
    void shrink_to_fit() { reallocate(m_size); }

    // iterators

    iterator begin() noexcept { return m_data; }

    const_iterator begin() const noexcept { return m_data; }

    const_iterator cbegin() const noexcept { return m_data; }

    iterator end() noexcept { return m_data + m_size; }

    const_iterator end() const noexcept { return m_data + m_size; }

    const_iterator cend() const noexcept { return m_data + m_size; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }

    const_reverse_iterator crbegin() const noexcept { return rbegin(); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

    const_reverse_iterator crend() const noexcept { return rend(); }

private:
    pointer allocate(size_type size) {
        auto data = static_cast<pointer>(m_allocator->allocate(value_size * size, value_align));
        // throw an exception if the allocation failed
        if (!data) {
            throw std::bad_alloc();
        }
        return data;
    }

    void ensure_element_fits() {
        if (m_size + 1 > m_capacity) {
            grow(m_size + 1);
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

        if (new_capacity < m_size) {
            m_size = new_capacity;
        }

        pointer new_data = nullptr;

        if (new_capacity > 0) {
            // ask the allocator for a new block of memory
            new_data = static_cast<pointer>(m_allocator->allocate(value_size * new_capacity, value_align));
            // throw an exception if the allocation failed
            if (!new_data) {
                throw std::bad_alloc();
            }
            // copy the old data to the new location
            std::memcpy(new_data, m_data, m_size * value_size);
        }

        if (m_data) {
            m_allocator->deallocate(m_data);
        }

        m_data = new_data;
        m_capacity = new_capacity;
    }

    void destruct_after(pointer new_end) {
        auto start = new_end - cbegin();
        for (size_type i = start; i < m_size; ++i) {
            m_data[i].~value_type();
        }
        m_size = start;
    }

    void deallocate() {
        if (!m_data) {
            return;
        }

        // destroy all the elements and deallocate the memory
        destruct_after(m_data);
        m_allocator->deallocate(m_data);

        // reset the data
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }

private:
    static constexpr auto value_size = sizeof(value_type);
    static constexpr auto value_align = alignof(value_type);

private:
    allocator* m_allocator;
    pointer m_data = nullptr;
    size_type m_size = 0;
    size_type m_capacity;
};

template <typename T>
void swap(array<T>& lhs, array<T>& rhs) {
    lhs.swap(rhs);
}

template <typename T>
bool operator==(const array<T>& lhs, const array<T>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T>
bool operator!=(const array<T>& lhs, const array<T>& rhs) {
    return !(lhs == rhs);
}

} // namespace containers

// bring symbols into parent namespace

using containers::array;

} // namespace shard

#endif // SHARD_CONTAINERS_ARRAY_HPP
