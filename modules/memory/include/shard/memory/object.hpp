// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_MEMORY_OBJECT_HPP
#define SHARD_MEMORY_OBJECT_HPP

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace shard {
namespace memory {

class object {
public:
    /// Default constructor
    constexpr object() = default;

    /// Copy constructor
    constexpr object(const object& /* other */)
    : m_ref_count(0) {}

    /// Get the number of references to this object
    constexpr std::size_t ref_count() const { return m_ref_count; }

    /// Increment the reference count by 1
    constexpr void retain() { ++m_ref_count; }

    /// Decrement the reference count by 1 and potentially free the memory
    void release(bool dealloc = true) {
        assert(m_ref_count > 0);
        if (--m_ref_count == 0 && dealloc) {
            delete this;
        }
    }

protected:
    virtual ~object() = default;

private:
    std::size_t m_ref_count = 0;
};

/// Wrapper to store a pointer with automatic reference counting
template <typename T>
class object_ptr {
    static_assert(std::is_base_of<object, T>(), "not an object");

public:
    using object_type = T;

public:
    /// Construct a 'null' valued pointer
    constexpr object_ptr()
    : m_pointer(nullptr) {};

    /// Copy constructor
    constexpr object_ptr(const object_ptr& other)
    : m_pointer(other.m_pointer) {
        if (m_pointer) {
            static_cast<object*>(m_pointer)->retain();
        }
    }

    /// Move constructor
    constexpr object_ptr(object_ptr&& other) noexcept
    : m_pointer(other.m_pointer) {
        other.m_pointer = nullptr;
    }

    /// Raw pointer constructor
    constexpr explicit object_ptr(object_type* ptr)
    : m_pointer(ptr) {
        if (m_pointer) {
            static_cast<object*>(m_pointer)->retain();
        }
    }

    /// Destructor
    ~object_ptr() {
        if (m_pointer) {
            static_cast<object*>(m_pointer)->release();
        }
    }

    /// Copy assignment operator
    object_ptr& operator=(const object_ptr& other) noexcept {
        if (this == &other) {
            return *this;
        }
        if (m_pointer != other.m_pointer) {
            if (m_pointer) {
                static_cast<object*>(m_pointer)->release();
            }
            if (other.m_pointer) {
                static_cast<object*>(other.m_pointer)->retain();
            }
            m_pointer = other.m_pointer;
        }
        return *this;
    }

    /// Move assignment operator
    object_ptr& operator=(object_ptr&& other) noexcept {
        if (this != &other) {
            if (m_pointer) {
                static_cast<object*>(m_pointer)->release();
            }
            m_pointer = other.m_pointer;
            other.m_pointer = nullptr;
        }
        return *this;
    }

    /// Raw pointer assignment operator
    object_ptr& operator=(object_type* ptr) noexcept {
        if (m_pointer != ptr) {
            if (m_pointer) {
                static_cast<object*>(m_pointer)->release();
            }
            if (ptr) {
                static_cast<object*>(ptr)->retain();
            }
            m_pointer = ptr;
        }
        return *this;
    }

    object_type* operator->() { return m_pointer; }

    const object_type* operator->() const { return m_pointer; }

    object_type& operator*() { return *m_pointer; }

    const object_type& operator*() const { return *m_pointer; }

    explicit operator object_type*() { return m_pointer; }

    object_type* get() { return m_pointer; }

    const object_type* get() const { return m_pointer; }

    explicit operator bool() const { return m_pointer != nullptr; }

private:
    object_type* m_pointer;
};

template <class T, class... Args>
object_ptr<T> make_object(Args&&... args) {
    return object_ptr<T>(new T(std::forward<Args>(args)...));
}

// comparisons between object pointers

template <typename T, typename U>
constexpr bool operator==(const object_ptr<T>& lhs, const object_ptr<U>& rhs) {
    return lhs.get() == rhs.get();
}

template <typename T, typename U>
constexpr bool operator!=(const object_ptr<T>& lhs, const object_ptr<U>& rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
constexpr bool operator<(const object_ptr<T>& lhs, const object_ptr<U>& rhs) {
    return lhs.get() < rhs.get();
}

template <typename T, typename U>
constexpr bool operator>(const object_ptr<T>& lhs, const object_ptr<U>& rhs) {
    return rhs < lhs;
}

template <typename T, typename U>
constexpr bool operator<=(const object_ptr<T>& lhs, const object_ptr<U>& rhs) {
    return !(rhs < lhs);
}

template <typename T, typename U>
constexpr bool operator>=(const object_ptr<T>& lhs, const object_ptr<U>& rhs) {
    return !(lhs < rhs);
}

// comparisons with nullptr

template <typename T>
constexpr bool operator==(const object_ptr<T>& o, std::nullptr_t) noexcept {
    return o.get() == nullptr;
}

template <typename T>
constexpr bool operator==(std::nullptr_t, const object_ptr<T>& o) noexcept {
    return o.get() == nullptr;
}

template <typename T>
constexpr bool operator!=(const object_ptr<T>& o, std::nullptr_t) noexcept {
    return o.get() != nullptr;
}

template <typename T>
constexpr bool operator!=(std::nullptr_t, const object_ptr<T>& o) noexcept {
    return o.get() != nullptr;
}

template <typename T>
constexpr bool operator<(const object_ptr<T>& o, std::nullptr_t) noexcept {
    return o.get() < nullptr;
}

template <typename T>
constexpr bool operator<(std::nullptr_t, const object_ptr<T>& o) noexcept {
    return nullptr < o.get();
}

template <typename T>
constexpr bool operator>(const object_ptr<T>& o, std::nullptr_t) noexcept {
    return nullptr < o;
}

template <typename T>
constexpr bool operator>(std::nullptr_t, const object_ptr<T>& o) noexcept {
    return o < nullptr;
}

template <typename T>
constexpr bool operator<=(const object_ptr<T>& o, std::nullptr_t) noexcept {
    return !(nullptr < o);
}

template <typename T>
constexpr bool operator<=(std::nullptr_t, const object_ptr<T>& o) noexcept {
    return !(o < nullptr);
}

template <typename T>
constexpr bool operator>=(const object_ptr<T>& o, std::nullptr_t) noexcept {
    return !(o < nullptr);
}

template <typename T>
constexpr bool operator>=(std::nullptr_t, const object_ptr<T>& o) noexcept {
    return !(nullptr < o);
}

// comparisons with a raw pointer

template <typename T, typename U>
constexpr bool operator==(const object_ptr<T>& o, const U* ptr) {
    return o.get() == ptr;
}

template <typename T, typename U>
constexpr bool operator==(const U* ptr, const object_ptr<T>& o) {
    return o == ptr;
}

template <typename T, typename U>
constexpr bool operator!=(const object_ptr<T>& o, const U* ptr) {
    return o.get() != ptr;
}

template <typename T, typename U>
constexpr bool operator!=(const U* ptr, const object_ptr<T>& o) {
    return o != ptr;
}

} // namespace memory

// bring symbols into parent namespace

using memory::make_object;
using memory::object;
using memory::object_ptr;

} // namespace shard

#endif // SHARD_MEMORY_OBJECT_HPP
