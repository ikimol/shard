// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_ANY_HPP
#define SHARD_ANY_HPP

#include <shard/meta/type_traits.hpp>

#include <type_traits>
#include <typeinfo>
#include <utility>

namespace shard {

class any {
public:
    /// Default constructor
    any() noexcept : m_holder(nullptr) {}

    /// Copy constructor
    any(const any& other) : m_holder(other.m_holder ? other.m_holder->clone() : nullptr) {}

    /// Move constructor
    any(any&& other) noexcept : m_holder(other.m_holder) {
        other.m_holder = nullptr;
    }

    /// Perfect forwarding constructor
    template <typename T, meta::disable_if_t<std::is_same<any&, T>::value>* = nullptr,
              meta::disable_if_t<std::is_const<T>::value>* = nullptr>
    any(T&& value) /* NOLINT */ : m_holder(new holder<std::decay_t<T>>(std::forward<T>(value))) {}

    /// Destructor
    ~any() noexcept {
        delete m_holder;
    }

    /// Copy assignment operator
    any& operator=(const any& other) {
        any(other).swap(*this);
        return *this;
    }

    /// Move assignment operator
    any& operator=(any&& other) noexcept {
        other.swap(*this);
        any().swap(other);
        return *this;
    }

    /// Perfect forwarding assignment operator
    template <typename T>
    any& operator=(T&& value) {
        any(std::forward<T>(value)).swap(*this);
        return *this;
    }

    /// Construct a value in-place
    template <typename T, typename... Args>
    std::decay_t<T>& emplace(Args&&... args) {
        reset();
        m_holder = new holder<std::decay_t<T>>(std::forward<Args>(args)...);
        return static_cast<holder<std::decay_t<T>>*>(m_holder)->value;
    }

    /// Delete the contained object
    void reset() noexcept {
        any().swap(*this);
    }

    /// Swap the contained object
    any& swap(any& other) noexcept {
        using std::swap;
        swap(m_holder, other.m_holder);
        return *this;
    }

    /// Check if an object is contained
    bool has_value() const noexcept {
        return m_holder != nullptr;
    }

    /// Get the type of the contained object
    const std::type_info& type() const noexcept {
        return m_holder ? m_holder->type() : typeid(void);
    }

private:
    class holder_base {
    public:
        virtual ~holder_base() = default;

        virtual const std::type_info& type() const noexcept = 0;

        virtual holder_base* clone() const = 0;
    };

    template <typename T>
    class holder : public holder_base {
    public:
        explicit holder(const T& value) : value(value) {}

        explicit holder(T&& value) : value(std::move(value)) {}

        template <typename... Args>
        explicit holder(Args&&... args) : value(std::forward<Args>(args)...) {}

        holder& operator=(const holder&) = delete;

        const std::type_info& type() const noexcept override {
            return typeid(T);
        }

        holder_base* clone() const override {
            return new holder(value);
        }

        T value;
    };

private:
    template <typename T>
    friend T* any_cast(any*) noexcept;

private:
    holder_base* m_holder;
};

/// Swap two objects
inline void swap(any& lhs, any& rhs) noexcept {
    lhs.swap(rhs);
}

class bad_any_cast : public std::bad_cast {
public:
    const char* what() const noexcept override {
        return "shard::bad_any_cast";
    }
};

template <typename T>
T* any_cast(any* a) noexcept {
    auto valid = a && a->type() == typeid(T);
    return valid ? std::addressof(static_cast<any::holder<std::remove_cv_t<T>>*>(a->m_holder)->value) : nullptr;
}

template <typename T>
inline const T* any_cast(const any* a) noexcept {
    return any_cast<T>(const_cast<any*>(a));
}

template <typename T>
T any_cast(any& a) {
    using non_ref_t = std::remove_reference_t<T>;
    auto result = any_cast<non_ref_t>(std::addressof(a));
    if (!result) {
        throw bad_any_cast();
    }
    using ref_t = std::conditional_t<std::is_reference<T>::value, T, std::add_lvalue_reference_t<T>>;
    return static_cast<ref_t>(*result);
}

template <typename T>
inline T any_cast(const any& a) {
    using non_ref_t = std::remove_reference_t<T>;
    return any_cast<const non_ref_t&>(const_cast<any&>(a));
}

} // namespace shard

#endif // SHARD_ANY_HPP
