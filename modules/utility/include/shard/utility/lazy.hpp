// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <functional>
#include <variant>

namespace shard {
namespace utility {

template <typename T>
class lazy {
public:
    using value_type = T;
    using init_type = std::function<value_type()>;

public:
    lazy(init_type init) /* NOLINT */
    : m_storage(init) {}

    lazy(value_type value) /* NOLINT */
    : m_storage(value) {}

    T& operator*() & {
        initialize();
        return std::get<value_type>(m_storage);
    }

    const T& operator*() const& {
        initialize();
        return std::get<value_type>(m_storage);
    }

    value_type* operator->() { return &(**this); }

    const value_type* operator->() const { return &(**this); }

private:
    using storage_type = std::variant<value_type, init_type>;

private:
    void initialize() const {
        if (std::holds_alternative<init_type>(m_storage)) {
            m_storage = std::get<init_type>(m_storage)();
        }
    }

private:
    mutable storage_type m_storage;
};

} // namespace utility

// bring symbols into parent namespace

using utility::lazy;

} // namespace shard
