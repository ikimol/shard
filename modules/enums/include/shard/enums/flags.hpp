// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_ENUMS_FLAGS_HPP
#define SHARD_ENUMS_FLAGS_HPP

#include "shard/enums/traits.hpp"

namespace shard {
namespace enums {

class flag {
public:
    using value_type = unsigned int;

public:
    constexpr flag(value_type value) /* NOLINT */
    : m_value(value) {}

    operator value_type() /* NOLINT */ const { return m_value; }

private:
    value_type m_value;
};

template <typename E>
class flags {
    static_assert(std::is_unsigned<std::underlying_type_t<E>>::value, "cannot use signed values as flags");

public:
    using enum_type = E;
    using value_type = std::underlying_type_t<E>;

public:
    constexpr flags() noexcept = default;

    constexpr flags(E flags) /* NOLINT */ noexcept
    : m_value(to_underlying(flags)) {}

    constexpr flags(value_type value) /* NOLINT */ noexcept
    : m_value(value) {}

    constexpr flags(flag f) /* NOLINT */ noexcept
    : m_value(f) {}

    constexpr flags& operator&=(value_type mask) noexcept {
        m_value &= mask;
        return *this;
    }

    constexpr flags& operator&=(enum_type e) noexcept {
        m_value &= to_underlying(e);
        return *this;
    }

    constexpr flags& operator|=(flags other) noexcept {
        m_value |= other.m_value;
        return *this;
    }

    constexpr flags& operator|=(enum_type e) noexcept {
        m_value |= to_underlying(e);
        return *this;
    }

    constexpr flags& operator^=(flags other) noexcept {
        m_value ^= other.m_value;
        return *this;
    }

    constexpr flags& operator^=(enum_type e) noexcept {
        m_value ^= to_underlying(e);
        return *this;
    }

    constexpr value_type value() const noexcept { return m_value; }

    constexpr operator value_type() /* NOLINT */ const noexcept { return m_value; }

    constexpr bool test(enum_type e) const noexcept {
        auto v = to_underlying(e);
        return (m_value & v) == v && (v != 0 || m_value == v);
    }

    constexpr flags& set(enum_type e, bool on = true) noexcept {
        return on ? (*this |= e) : (*this &= ~to_underlying(e));
    }

private:
    value_type m_value = 0;
};

template <typename E>
constexpr flags<E> operator|(flags<E> lhs, flags<E> rhs) noexcept {
    return flag(lhs.value() | rhs.value());
}

template <typename E>
constexpr flags<E> operator|(flags<E> lhs, typename flags<E>::enum_type e) noexcept {
    return flag(lhs.value() | to_underlying(e));
}

template <typename E>
constexpr flags<E> operator^(flags<E> lhs, flags<E> rhs) noexcept {
    return flag(lhs.value() ^ rhs.value());
}

template <typename E>
constexpr flags<E> operator^(flags<E> lhs, typename flags<E>::enum_type e) noexcept {
    return flag(lhs.value() ^ to_underlying(e));
}

template <typename E>
constexpr flags<E> operator&(flags<E> lhs, typename flags<E>::value_type mask) noexcept {
    return flag(lhs.value() & mask);
}

template <typename E>
constexpr flags<E> operator&(flags<E> lhs, typename flags<E>::enum_type e) noexcept {
    return flag(lhs.value() & to_underlying(e));
}

template <typename E>
constexpr flags<E> operator~(flags<E> flags) noexcept {
    return flag(~flags.value());
}

} // namespace enums

// bring symbols into parent namespace

using enums::flags;

} // namespace shard

#define SHARD_DECLARE_FLAGS(T, E) using T = shard::flags<E>; // NOLINT

#define SHARD_DECLARE_FLAG_OPERATORS(T)                                                                                \
    constexpr inline shard::flags<T::enum_type> operator|(T::enum_type lhs, T::enum_type rhs) noexcept {               \
        return shard::flags<T::enum_type>(lhs) | rhs;                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    constexpr inline shard::flags<T::enum_type> operator|(T::enum_type lhs, shard::flags<T::enum_type> rhs) noexcept { \
        return rhs | lhs;                                                                                              \
    }

#endif // SHARD_ENUMS_FLAGS_HPP
