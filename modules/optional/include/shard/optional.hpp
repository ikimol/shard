// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_OPTIONAL_HPP
#define SHARD_OPTIONAL_HPP

#include <shard/meta.hpp>

#include <new>
#include <stdexcept>
#include <type_traits>

namespace shard {

template <typename T>
class optional;

struct nullopt_t {
    explicit constexpr nullopt_t(int) {}
};

constexpr nullopt_t nullopt {0};

class bad_optional_access : public std::runtime_error {
public:
    bad_optional_access() : std::runtime_error("shard::bad_optional_access") {}
};

template <typename T>
class optional {
    static_assert(!std::is_reference<T>::value, "optional may not be used with reference types");
    static_assert(!std::is_abstract<T>::value, "optional may not be used with abstract types");
    static_assert(!std::is_same<T, meta::in_place_t>::value, "optional may not be used with in_place_t");
    static_assert(!std::is_same<T, nullopt_t>::value, "optional may not be used with nullopt_t");
    static_assert(std::is_destructible<T>::value, "optional may not be used with non-destructible types");

public:
    using value_type = T;

public:
    /// Default constructor
    constexpr optional() noexcept = default;

    /// Empty constructor
    constexpr optional(nullopt_t) /* NOLINT */ noexcept {}

    /// Copy constructor
    constexpr optional(const optional& other) noexcept(std::is_nothrow_copy_constructible<T>::value) {
        if (other.has_value()) {
            construct(other.value());
        }
    }

    /// Move constructor
    constexpr optional(optional&& other) noexcept(std::is_nothrow_move_constructible<T>::value) {
        if (other.has_value()) {
            construct(std::move(other.value()));
            other.reset();
        }
    }

    /// Value converting constructor
    template <typename U = value_type, typename = std::enable_if_t<!std::is_same<std::decay_t<U>, optional<T>>::value>>
    constexpr optional(U&& value) /* NOLINT */ {
        construct(std::forward<U>(value));
    }

    /// Variadic value constructor
    template <typename... Args>
    explicit optional(meta::in_place_t, Args&&... args) noexcept(std::is_nothrow_constructible<T, Args...>::value) {
        construct(std::forward<Args>(args)...);
    }

    /// Empty assignment operator
    optional& operator=(nullopt_t) noexcept {
        reset();
        return *this;
    }

    /// Copy assignment operator
    constexpr optional& operator=(const optional& other) noexcept(std::is_nothrow_copy_assignable<T>::value) {
        if (this != &other) {
            if (other.has_value()) {
                assign(other.value());
            } else {
                reset();
            }
        }
        return *this;
    }

    /// Move assignment operator
    constexpr optional& operator=(optional&& other) noexcept(std::is_nothrow_move_assignable<T>::value) {
        if (this != &other) {
            if (other.has_value()) {
                assign(std::move(other.value()));
                other.reset();
            } else {
                reset();
            }
        }
        return *this;
    }

    /// Value converting assignment operator
    template <typename U = value_type, typename = std::enable_if_t<!std::is_same<std::decay_t<U>, optional<T>>::value>>
    optional& operator=(U&& value) {
        assign(std::forward<U>(value));
        return *this;
    }

    template <typename... Args>
    value_type& emplace(Args&&... args) {
        reset();
        construct(std::forward<Args>(args)...);
        return value();
    }

    void reset() noexcept { m_storage.clear(); }

    void swap(optional& other) {
        if (has_value() && other.has_value()) {
            using std::swap;
            swap(value(), other.value());
        } else if (has_value()) {
            other.emplace(std::move(value()));
            reset();
        } else if (other.has_value()) {
            emplace(std::move(other.value()));
            other.reset();
        }
    }

    constexpr const value_type& value() const& {
        assert_value();
        return m_storage.value;
    }

    constexpr value_type& value() & {
        assert_value();
        return m_storage.value;
    }

    constexpr value_type&& value() && {
        assert_value();
        return std::move(m_storage.value);
    }

    constexpr const value_type&& value() const&& {
        assert_value();
        return std::move(m_storage.value);
    }

    template <typename U>
    value_type value_or(U&& default_value) {
        return has_value() ? value() : static_cast<value_type>(std::forward<U>(default_value));
    }

    /// Check if this option has value
    constexpr bool has_value() const noexcept { return m_storage.has_value; }

    /// Check if this option has value
    constexpr explicit operator bool() const noexcept { return has_value(); }

    constexpr value_type& operator*() { return m_storage.value; }

    constexpr const value_type& operator*() const { return m_storage.value; }

    constexpr value_type* operator->() { return std::addressof(m_storage.value); }

    constexpr const value_type* operator->() const { return std::addressof(m_storage.value); }

private:
    template <typename... Args>
    void construct(Args&&... args) {
        const void* p = &m_storage.value;
        ::new (const_cast<void*>(p)) value_type(std::forward<Args>(args)...);
        m_storage.has_value = true;
    }

    void assign(const value_type& value) {
        if (has_value()) {
            m_storage.value = value;
        } else {
            construct(value);
        }
    }

    void assert_value() const {
        if (!has_value()) {
            throw bad_optional_access();
        }
    }

private:
    struct trivially_destructible_storage {
        union {
            char empty_state;
            value_type value;
        };
        bool has_value = false;

        constexpr trivially_destructible_storage() noexcept : empty_state('\0') {}

        void clear() { has_value = false; }
    };

    struct non_trivially_destructible_storage {
        union {
            char empty_state;
            value_type value;
        };
        bool has_value = false;

        non_trivially_destructible_storage() noexcept : empty_state('\0') {}

        ~non_trivially_destructible_storage() { clear(); }

        void clear() {
            if (has_value) {
                value.~value_type();
                has_value = false;
            }
        }
    };

    // clang-format off
    using storage_type = std::conditional_t<std::is_trivially_destructible<value_type>::value,
                                         trivially_destructible_storage,
                                         non_trivially_destructible_storage
                                         >;
    // clang-format on

private:
    storage_type m_storage;
};

template <typename T>
void swap(optional<T>& lhs, optional<T>& rhs) {
    lhs.swap(rhs);
}

template <typename T>
constexpr optional<std::decay_t<T>> make_optional(T&& value) {
    return optional<std::decay_t<T>>(std::forward<T>(value));
}

template <typename T, typename... Args>
constexpr optional<T> make_optional(Args&&... args) {
    return optional<T>(meta::in_place, std::forward<Args>(args)...);
}

// comparisons between optionals

template <typename T, typename U>
constexpr bool operator==(const optional<T>& lhs, const optional<U>& rhs) {
    if (lhs.has_value() != rhs.has_value()) {
        return false;
    }
    if (!lhs.has_value()) {
        return true;
    }
    return *lhs == *rhs;
}

template <typename T, typename U>
constexpr bool operator!=(const optional<T>& lhs, const optional<U>& rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
constexpr bool operator<(const optional<T>& lhs, const optional<U>& rhs) {
    if (!rhs.has_value()) {
        return false;
    }
    if (!lhs.has_value()) {
        return true;
    }
    return *lhs < *rhs;
}

template <typename T, typename U>
constexpr bool operator>(const optional<T>& lhs, const optional<U>& rhs) {
    return rhs < lhs;
}

template <typename T, typename U>
constexpr bool operator<=(const optional<T>& lhs, const optional<U>& rhs) {
    return !(rhs < lhs);
}

template <typename T, typename U>
constexpr bool operator>=(const optional<T>& lhs, const optional<U>& rhs) {
    return !(lhs < rhs);
}

// comparisons with nullopt

template <typename T>
constexpr bool operator==(const optional<T>& o, nullopt_t) noexcept {
    return !o.has_value();
}

template <typename T>
constexpr bool operator==(nullopt_t, const optional<T>& o) noexcept {
    return !o.has_value();
}

template <typename T>
constexpr bool operator!=(const optional<T>& o, nullopt_t) noexcept {
    return o.has_value();
}

template <typename T>
constexpr bool operator!=(nullopt_t, const optional<T>& o) noexcept {
    return o.has_value();
}

template <typename T>
constexpr bool operator<(const optional<T>&, nullopt_t) noexcept {
    return false;
}

template <typename T>
constexpr bool operator<(nullopt_t, const optional<T>& o) noexcept {
    return o.has_value();
}

template <typename T>
constexpr bool operator>(const optional<T>& o, nullopt_t) noexcept {
    return o.has_value();
}

template <typename T>
constexpr bool operator>(nullopt_t, const optional<T>&) noexcept {
    return false;
}

template <typename T>
constexpr bool operator<=(const optional<T>& o, nullopt_t) noexcept {
    return !o.has_value();
}

template <typename T>
constexpr bool operator<=(nullopt_t, const optional<T>&) noexcept {
    return true;
}

template <typename T>
constexpr bool operator>=(const optional<T>&, nullopt_t) noexcept {
    return true;
}

template <typename T>
constexpr bool operator>=(nullopt_t, const optional<T>& o) noexcept {
    return !o.has_value();
}

// comparisons with a type

template <typename T, typename U>
constexpr bool operator==(const optional<T>& o, const U& value) {
    return o.has_value() && *o == value;
}

template <typename T, typename U>
constexpr bool operator==(const T& value, const optional<U>& o) {
    return o == value;
}

template <typename T, typename U>
constexpr bool operator!=(const optional<T>& o, const U& value) {
    return !(o == value);
}

template <typename T, typename U>
constexpr bool operator!=(const T& value, const optional<U>& o) {
    return !(value == o);
}

template <typename T, typename U>
constexpr bool operator<(const optional<T>& o, const U& value) {
    return o.has_value() && *o < value;
}

template <typename T, typename U>
constexpr bool operator<(const T& value, const optional<U>& o) {
    return o.has_value() && value < *o;
}

template <typename T, typename U>
constexpr bool operator<=(const optional<T>& o, const U& value) {
    return !(value < *o);
}

template <typename T, typename U>
constexpr bool operator<=(const T& value, const optional<U>& o) {
    return !(*o < value);
}

template <typename T, typename U>
constexpr bool operator>(const optional<T>& o, const U& value) {
    return value < *o;
}

template <typename T, typename U>
constexpr bool operator>(const T& value, const optional<U>& o) {
    return *o < value;
}

template <typename T, typename U>
constexpr bool operator>=(const optional<T>& o, const U& value) {
    return !(*o < value);
}

template <typename T, typename U>
constexpr bool operator>=(const T& value, const optional<U>& o) {
    return !(value < *o);
}

} // namespace shard

// std::hash compatibility

namespace std {

template <typename T>
struct hash<shard::optional<T>> {
    std::size_t operator()(const shard::optional<T>& o) const {
        return o.has_value() ? hash<remove_const_t<T>> {}(*o) : 0;
    }
};
} // namespace std

#endif // SHARD_OPTIONAL_HPP
