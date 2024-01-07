// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/expected/bad_expected_access.hpp"
#include "shard/expected/traits.hpp"
#include "shard/expected/unexpected.hpp"

#include <shard/memory/utils.hpp>
#include <shard/meta/type_traits.hpp>
#include <shard/utility/exception_guard.hpp>

#include <cassert>
#include <memory>

namespace shard {

template <typename T, typename E>
class expected {
public:
    using value_type = T;
    using error_type = E;
    using unexpected_type = unexpected<E>;
    template <typename T2>
    using rebind = expected<T2, E>;

private:
    template <typename T2, typename E2>
    using is_convertible =
        std::conjunction<std::is_constructible<T2, const T2&>,
                         std::is_constructible<E2, const E2&>,
                         std::negation<std::is_constructible<T, expected<T2, E2>&>>,
                         std::negation<std::is_constructible<T, expected<T2, E2>>>,
                         std::negation<std::is_constructible<T, const expected<T2, E2>&>>,
                         std::negation<std::is_constructible<T, const expected<T2, E2>>>,
                         std::negation<std::is_convertible<expected<T2, E2>&, T>>,
                         std::negation<std::is_convertible<expected<T2, E2>&&, T>>,
                         std::negation<std::is_convertible<const expected<T2, E2>&, T>>,
                         std::negation<std::is_convertible<const expected<T2, E2>&&, T>>,
                         std::negation<std::is_constructible<unexpected<E>, expected<T2, E2>&>>,
                         std::negation<std::is_constructible<unexpected<E>, expected<T2, E2>>>,
                         std::negation<std::is_constructible<unexpected<E>, const expected<T2, E2>&>>,
                         std::negation<std::is_constructible<unexpected<E>, const expected<T2, E2>>>>;

public:
    /// Default constructor
    expected() noexcept
    : m_has_value(true) {
        memory::construct_at(std::addressof(m_storage.value));
    }

    /// Copy constructor
    expected(const expected& other)
    : m_has_value(other.m_has_value) {
        if (m_has_value) {
            memory::construct_at(std::addressof(m_storage.value), other.m_storage.value);
        } else {
            memory::construct_at(std::addressof(m_storage.error), other.m_storage.error);
        }
    }

    /// Move constructor
    expected(expected&& other) noexcept
    : m_has_value(other.m_has_value) {
        if (m_has_value) {
            memory::construct_at(std::addressof(m_storage.value), std::move(other.m_storage.value));
        } else {
            memory::construct_at(std::addressof(m_storage.error), std::move(other.m_storage.error));
        }
    }

    /// Converting copy constructor
    template <typename T2, typename E2, typename = std::enable_if_t<is_convertible<T2, E2>::value>>
    explicit expected(const expected<T2, E2>& other) /* NOLINT */
    : m_has_value(other.m_has_value) {
        if (m_has_value) {
            memory::construct_at(std::addressof(m_storage.value), other.m_storage.value);
        } else {
            memory::construct_at(std::addressof(m_storage.error), other.m_storage.error);
        }
    }

    /// Converting move constructor
    template <typename T2, typename E2, typename = std::enable_if_t<is_convertible<T2, E2>::value>>
    /* implicit */ expected(expected<T2, E2>&& other) noexcept /* NOLINT */
    : m_has_value(other.m_has_value) {
        if (m_has_value) {
            memory::construct_at(std::addressof(m_storage.value), std::move(other.m_storage.value));
        } else {
            memory::construct_at(std::addressof(m_storage.error), std::move(other.m_storage.error));
        }
    }

    /// Copying constructor for an unexpected value
    template <typename E2>
    /* implicit */ expected(const unexpected<E2>& ue) /* NOLINT */
    : m_has_value(false) {
        memory::construct_at(std::addressof(m_storage.error), ue.error());
    }

    /// Moving constructor for an unexpected value
    template <typename E2>
    /* implicit */ expected(unexpected<E2>&& ue) /* NOLINT */
    : m_has_value(false) {
        memory::construct_at(std::addressof(m_storage.error), std::move(ue.error()));
    }

    /// Converting constructor for a value (non-error)
    template <
        typename T2 = T,
        typename = std::enable_if_t<!std::is_same_v<shard::unqualified_t<T2>, std::in_place_t>
                                    && !std::is_same_v<shard::unqualified_t<T2>, expected>
                                    && !is_unexpected_v<shard::unqualified_t<T2>> && std::is_constructible_v<T2, T2>>>
    /* implicit */ expected(T2&& value) /* NOLINT */
    : m_has_value(true) {
        memory::construct_at(std::addressof(m_storage.value), std::forward<T2>(value));
    }

    /// Destructor
    ~expected() {
        if (m_has_value) {
            std::destroy_at(std::addressof(m_storage.value));
        } else {
            std::destroy_at(std::addressof(m_storage.error));
        }
    }

    /// Copy assignment operator
    expected& operator=(const expected& other) {
        if (m_has_value && other.m_has_value) {
            m_storage.value = other.m_storage.value;
        } else if (m_has_value) {
            reinit(m_storage.error, m_storage.value, other.m_storage.error);
        } else if (other.m_has_value) {
            reinit(m_storage.value, m_storage.error, other.m_storage.value);
        } else {
            m_storage.error = other.m_storage.error;
        }
        m_has_value = other.m_has_value;
        return *this;
    }

    /// Move assignment operator
    expected& operator=(expected&& other) noexcept {
        if (m_has_value && other.m_has_value) {
            m_storage.value = std::move(other.m_storage.value);
        } else if (m_has_value) {
            reinit(m_storage.error, m_storage.value, std::move(other.m_storage.error));
        } else if (other.m_has_value) {
            reinit(m_storage.value, m_storage.error, std::move(other.m_storage.value));
        } else {
            m_storage.error = std::move(other.m_storage.error);
        }
        m_has_value = other.m_has_value;
        return *this;
    }

    /// Copying assignment operator for an unexpected value
    template <typename E2>
    expected& operator=(const unexpected<E2>& ue) {
        if (m_has_value) {
            reinit(m_storage.error, m_storage.value, ue.error());
            m_has_value = false;
        } else {
            m_storage.error = ue.error();
        }
        return *this;
    }

    /// Moving assignment operator for an unexpected value
    template <typename E2>
    expected& operator=(unexpected<E2>&& ue) {
        if (m_has_value) {
            reinit(m_storage.error, m_storage.value, std::move(ue.error()));
            m_has_value = false;
        } else {
            m_storage.error = std::move(ue.error());
        }
        return *this;
    }

    /// Converting assignment operator for a value (non-error)
    template <typename T2 = T,
              typename = std::enable_if_t<!std::is_same_v<expected, shard::unqualified_t<T2>>
                                          && !is_unexpected_v<shard::unqualified_t<T2>>
                                          && std::is_constructible_v<T, T2> && std::is_assignable_v<T&, T2>>>
    expected& operator=(T2&& value) noexcept {
        if (m_has_value) {
            m_storage.value = std::forward<T2>(value);
        } else {
            reinit(m_storage.value, m_storage.error, std::forward<T2>(value));
            m_has_value = true;
        }
        return *this;
    }

    /// Access the value
    const value_type& value() const {
        if (!m_has_value) {
            throw bad_expected_access(std::move(m_storage.error));
        }
        return m_storage.value;
    }

    /// Get the value if available or the provided default value
    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
    T value_or(U&& value) {
        return m_has_value ? m_storage.value : static_cast<T>(std::forward<U>(value));
    }

    /// Access the error
    const error_type& error() const {
        assert(!m_has_value);
        return m_storage.error;
    }

    /// Check if a value is stored
    bool has_value() const { return m_has_value; }

    /// Check if a value is stored
    explicit operator bool() const { return has_value(); }

    /// Access the underlying value
    value_type* operator->() noexcept {
        assert(m_has_value);
        return std::addressof(m_storage.value);
    }

    /// Access the underlying value
    const value_type* operator->() const noexcept {
        assert(m_has_value);
        return std::addressof(m_storage.value);
    }

    /// Access the underlying value
    value_type& operator*() noexcept {
        assert(m_has_value);
        return m_storage.value;
    }

    /// Access the underlying value
    const value_type& operator*() const noexcept {
        assert(m_has_value);
        return m_storage.value;
    }

    // monadic operations

    /// Execute the provided function if a value is contained
    template <typename F>
    auto and_then(F&& f) {
        using U = unqualified_t<std::invoke_result_t<F, const T&>>;
        static_assert(is_expected_v<U>);
        static_assert(std::is_same_v<typename U::error_type, E>);
        if (m_has_value) {
            return std::invoke(std::forward<F>(f), m_storage.value);
        }
        return U(unexpected(m_storage.error));
    }

    /// Execute the provided function if an error is contained
    template <typename F>
    auto or_else(F&& f) {
        using U = unqualified_t<std::invoke_result_t<F, const E&>>;
        static_assert(is_expected_v<U>);
        static_assert(std::is_same_v<typename U::value_type, T>);
        if (m_has_value) {
            return U(m_storage.value);
        }
        return std::invoke(std::forward<F>(f), m_storage.error);
    }

    // equality

    /// Equality with another expected
    template <typename T2, typename E2>
    friend bool operator==(const expected& lhs, const expected<T2, E2>& rhs) {
        if (lhs.m_has_value != rhs.m_has_value) {
            return false;
        } else {
            if (lhs.m_has_value) {
                return lhs.m_storage.value == rhs.m_storage.value;
            } else {
                return lhs.m_storage.error == rhs.m_storage.error;
            }
        }
    }

    /// Inequality with another expected
    template <typename T2, typename E2>
    friend bool operator!=(const expected& lhs, const expected<T2, E2>& rhs) {
        return !(lhs == rhs); /* NOLINT */
    }

    /// Equality with a value
    template <typename T2>
    friend bool operator==(const expected& ex, const T2& value) {
        return ex.m_has_value && static_cast<bool>(ex.m_storage.value == value);
    }

    /// Inequality with a value
    template <typename T2>
    friend bool operator!=(const expected& ex, const T2& value) {
        return !(ex == value); /* NOLINT */
    }

    /// Equality with an error
    template <typename E2>
    friend bool operator==(const expected& ex, const unexpected<E2>& ue) {
        return !ex.m_has_value && static_cast<bool>(ex.m_storage.error == ue.error());
    }

    /// Inequality with an error
    template <typename E2>
    friend bool operator!=(const expected& ex, const unexpected<E2>& ue) {
        return !(ex == ue); /* NOLINT */
    }

private:
    template <typename T1, typename T2, typename... Args>
    static void reinit(T1& new_value, T2& old_value, Args&&... args) {
        if constexpr (std::is_nothrow_constructible_v<T1, Args...>) {
            std::destroy_at(std::addressof(old_value));
            memory::construct_at(std::addressof(new_value), std::forward<Args>(args)...);
        } else if (std::is_nothrow_move_constructible_v<T1>) {
            T1 tmp(std::forward<Args>(args)...);
            std::destroy_at(std::addressof(old_value));
            memory::construct_at(std::addressof(new_value), std::move(tmp));
        } else {
            static_assert(std::is_nothrow_move_constructible_v<T2>);
            T2 tmp(std::move(old_value));
            std::destroy_at(std::addressof(old_value));
            auto guard = shard::make_exception_guard([&] {
                // restore old value in case new one cannot be constructed
                memory::construct_at(std::addressof(old_value), std::move(tmp));
            });
            memory::construct_at(std::addressof(new_value), std::forward<Args>(args)...);
            guard.dismiss();
        }
    }

private:
    bool m_has_value;

    union storage {
        storage() {} // handled by expected()

        ~storage() {} // handled by ~expected()

        value_type value;
        error_type error;
    } m_storage;
};

// void specialization

template <typename E>
class expected<void, E> {
public:
    using value_type = void;
    using error_type = E;
    using unexpected_type = unexpected<E>;
    template <typename T2>
    using rebind = expected<T2, E>;

private:
    template <typename T2, typename E2>
    using is_convertible =
        std::conjunction<std::is_void<T2>,
                         std::is_constructible<E2, const E2&>,
                         std::negation<std::is_constructible<unexpected<E>, expected<T2, E2>&>>,
                         std::negation<std::is_constructible<unexpected<E>, expected<T2, E2>>>,
                         std::negation<std::is_constructible<unexpected<E>, const expected<T2, E2>&>>,
                         std::negation<std::is_constructible<unexpected<E>, const expected<T2, E2>>>>;

public:
    /// Default constructor
    expected() noexcept
    : m_has_value(true) {}

    /// Copy constructor
    expected(const expected& other)
    : m_has_value(other.m_has_value) {
        if (!m_has_value) {
            memory::construct_at(std::addressof(m_storage.error), other.m_storage.error);
        }
    }

    /// Move constructor
    expected(expected&& other) noexcept
    : m_has_value(other.m_has_value) {
        if (!m_has_value) {
            memory::construct_at(std::addressof(m_storage.error), std::move(other.m_storage.error));
        }
    }

    /// Converting copy constructor
    template <typename T2, typename E2, typename = std::enable_if_t<is_convertible<T2, E2>::value>>
    explicit expected(const expected<T2, E2>& other) /* NOLINT */
    : m_has_value(other.m_has_value) {
        if (!m_has_value) {
            memory::construct_at(std::addressof(m_storage.error), other.m_storage.error);
        }
    }

    /// Converting move constructor
    template <typename T2, typename E2, typename = std::enable_if_t<is_convertible<T2, E2>::value>>
    /* implicit */ expected(expected<T2, E2>&& other) noexcept /* NOLINT */
    : m_has_value(other.m_has_value) {
        if (!m_has_value) {
            memory::construct_at(std::addressof(m_storage.error), std::move(other.m_storage.error));
        }
    }

    /// Copying constructor for an unexpected value
    template <typename E2>
    /* implicit */ expected(const unexpected<E2>& ue) /* NOLINT */
    : m_has_value(false) {
        memory::construct_at(std::addressof(m_storage.error), ue.error());
    }

    /// Moving constructor for an unexpected value
    template <typename E2>
    /* implicit */ expected(unexpected<E2>&& ue) /* NOLINT */
    : m_has_value(false) {
        memory::construct_at(std::addressof(m_storage.error), std::move(ue.error()));
    }

    /// Destructor
    ~expected() {
        if (!m_has_value) {
            std::destroy_at(std::addressof(m_storage.error));
        }
    }

    /// Copy assignment operator
    expected& operator=(const expected& other) {
        if (m_has_value) {
            if (!other.m_has_value) {
                memory::construct_at(std::addressof(m_storage.error), other.m_storage.error);
                m_has_value = false;
            }
        } else {
            if (other.m_has_value) {
                std::destroy_at(std::addressof(m_storage.error));
                m_has_value = true;
            } else {
                m_storage.error = other.m_storage.error;
            }
        }
        return *this;
    }

    /// Move assignment operator
    expected& operator=(expected&& other) noexcept {
        if (m_has_value) {
            if (!other.m_has_value) {
                memory::construct_at(std::addressof(m_storage.error), std::move(other.m_storage.error));
                m_has_value = false;
            }
        } else {
            if (other.m_has_value) {
                std::destroy_at(std::addressof(m_storage.error));
                m_has_value = true;
            } else {
                m_storage.error = std::move(other.m_storage.error);
            }
        }
        return *this;
    }

    /// Copying assignment operator for an unexpected value
    template <typename E2>
    expected& operator=(const unexpected<E2>& ue) {
        if (m_has_value) {
            memory::construct_at(std::addressof(m_storage.error), ue.error());
            m_has_value = false;
        } else {
            m_storage.error = ue.error();
        }
        return *this;
    }

    /// Moving assignment operator for an unexpected value
    template <typename E2>
    expected& operator=(unexpected<E2>&& ue) {
        if (m_has_value) {
            memory::construct_at(std::addressof(m_storage.error), std::move(ue.error()));
            m_has_value = false;
        } else {
            m_storage.error = std::move(ue.error());
        }
        return *this;
    }

    /// Access the value
    void value() const {
        if (!m_has_value) {
            throw bad_expected_access(std::move(m_storage.error));
        }
    }

    /// Access the error
    const error_type& error() const {
        assert(!m_has_value);
        return m_storage.error;
    }

    /// Check if a value is stored
    bool has_value() const { return m_has_value; }

    /// Check if a value is stored
    explicit operator bool() const { return m_has_value; }

    // monadic operations

    /// Execute the provided function if a value is contained
    template <typename F>
    auto and_then(F&& f) {
        using U = unqualified_t<std::invoke_result_t<F>>;
        static_assert(is_expected_v<U>);
        static_assert(std::is_same_v<typename U::error_type, E>);
        if (m_has_value) {
            return std::invoke(std::forward<F>(f));
        }
        return U(unexpected(m_storage.error));
    }

    /// Execute the provided function if an error is contained
    template <typename F>
    auto or_else(F&& f) {
        using U = unqualified_t<std::invoke_result_t<F, const E&>>;
        static_assert(is_expected_v<U>);
        static_assert(std::is_same_v<typename U::value_type, void>);
        if (m_has_value) {
            return U();
        }
        return std::invoke(std::forward<F>(f), m_storage.error);
    }

    // equality

    /// Equality with another expected
    template <typename T2, typename E2, typename = std::enable_if_t<std::is_void_v<T2>>>
    friend bool operator==(const expected& lhs, const expected<T2, E2>& rhs) {
        if (lhs.m_has_value != rhs.m_has_value) {
            return false;
        } else {
            return lhs.m_has_value || lhs.m_storage.error == rhs.m_storage.error;
        }
    }

    /// Inquality with another expected
    template <typename T2, typename E2, typename = std::enable_if_t<std::is_void_v<T2>>>
    friend bool operator!=(const expected& lhs, const expected<T2, E2>& rhs) {
        return !(lhs == rhs); /* NOLINT */
    }

    /// Equality with an error
    template <typename E2>
    friend bool operator==(const expected& ex, const unexpected<E2>& ue) {
        return !ex.m_has_value && static_cast<bool>(ex.m_storage.error == ue.error());
    }

    /// Inequality with an error
    template <typename E2>
    friend bool operator!=(const expected& ex, const unexpected<E2>& ue) {
        return !(ex == ue); /* NOLINT */
    }

private:
    bool m_has_value;

    union storage {
        storage() {} // handled by expected()

        ~storage() {} // handled by ~expected()

        error_type error;
    } m_storage;
};

} // namespace shard
