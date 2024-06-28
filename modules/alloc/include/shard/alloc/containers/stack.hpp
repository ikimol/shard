// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/alloc/containers/array.hpp"

namespace shard {
namespace containers {

template <typename T>
class stack {
    friend bool operator==(const stack&, const stack&);

public:
    using container_type = array<T>;
    using value_type = typename container_type::value_type;
    using size_type = typename container_type::size_type;
    using reference = typename container_type::reference;
    using const_reference = typename container_type::const_reference;

public:
    explicit stack(allocator& a)
    : m_array(a) {}

    // modifiers

    /// Add a new element to the top of the stack
    void push(const_reference value) { m_array.append(value); }

    /// Add a new element to the top of the stack
    void push(value_type&& value) { m_array.append(std::move(value)); }

    /// Create a new element in-place on the top of the stack
    template <typename... Args>
    void emplace(Args&&... args) {
        m_array.emplace_back(std::forward<Args>(args)...);
    }

    /// Remove the element from the top of the stack
    void pop() { m_array.remove_last(); }

    /// Exchange the contents of the stack with those of another
    void swap(stack& other) {
        using std::swap;
        swap(m_array, other.m_array);
    }

    // element access

    /// Get the element on the top of the stack
    reference top() { return m_array.last(); }

    /// Get the element on the top of the stack
    const_reference top() const { return m_array.last(); }

    // size & capacity

    /// Get the number of elements in the array
    size_type size() const { return m_array.size(); }

    /// Check if the array is empty
    bool is_empty() const { return m_array.is_empty(); }

    // for STL compatibility
    bool empty() const { return is_empty(); }

private:
    array<T> m_array;
};

template <typename T>
void swap(stack<T>& lhs, stack<T>& rhs) {
    lhs.swap(rhs);
}

template <typename T>
bool operator==(const stack<T>& lhs, const stack<T>& rhs) {
    return lhs.m_array == rhs.m_array;
}

template <typename T>
bool operator!=(const stack<T>& lhs, const stack<T>& rhs) {
    return !(lhs == rhs);
}

} // namespace containers

// bring symbols into parent namespace

using containers::stack;

} // namespace shard
