// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#define SHARD_RUNTIME_ID(name, T, ...)                                                                                 \
    class name {                                                                                                       \
        friend bool operator==(const name&, const name&);                                                              \
        friend bool operator<(const name&, const name&);                                                               \
                                                                                                                       \
    public:                                                                                                            \
        using value_type = T;                                                                                          \
                                                                                                                       \
    public:                                                                                                            \
        static name next() {                                                                                           \
            return name(s_counter++);                                                                                  \
        }                                                                                                              \
                                                                                                                       \
        [[nodiscard]] value_type value() const {                                                                       \
            return m_value;                                                                                            \
        }                                                                                                              \
                                                                                                                       \
    private:                                                                                                           \
        explicit name(value_type value)                                                                                \
        : m_value(value) {}                                                                                            \
                                                                                                                       \
    private:                                                                                                           \
        static inline value_type s_counter = {};                                                                       \
                                                                                                                       \
        value_type m_value;                                                                                            \
    };                                                                                                                 \
                                                                                                                       \
    inline bool operator==(const name& lhs, const name& rhs) {                                                 \
        return lhs.m_value == rhs.m_value;                                                                             \
    }                                                                                                                  \
    inline bool operator!=(const name& lhs, const name& rhs) {                                                 \
        return !(lhs == rhs);                                                                                          \
    }                                                                                                                  \
    inline bool operator<(const name& lhs, const name& rhs) {                                                  \
        return lhs.m_value < rhs.m_value;                                                                              \
    }                                                                                                                  \
    inline bool operator<=(const name& lhs, const name& rhs) {                                                 \
        return !(rhs < lhs);                                                                                           \
    }                                                                                                                  \
    inline bool operator>(const name& lhs, const name& rhs) {                                                  \
        return rhs < lhs;                                                                                              \
    }                                                                                                                  \
    inline bool operator>=(const name& lhs, const name& rhs) {                                                 \
        return !(lhs < rhs);                                                                                           \
    }

#define SHARD_RUNTIME_ID_HASH_SUPPORT(ns, name)                                                                        \
    template <>                                                                                                        \
    struct std::hash<ns::name> {                                                                                       \
        std::size_t operator()(const ns::name& id) const noexcept {                                                    \
            return std::hash<ns::name::value_type>()(id.value());                                                      \
        }                                                                                                              \
    };
