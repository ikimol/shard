// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <utility>

namespace shard {
namespace utility {

template <typename Rollback>
class exception_guard {
public:
    using rollback_type = Rollback;

public:
    exception_guard() = delete;

    explicit exception_guard(rollback_type rollback)
    : m_rollback(std::move(rollback))
    , m_active(true) {}

    exception_guard(const exception_guard&) = delete;

    exception_guard(exception_guard&& other) noexcept(std::is_nothrow_move_constructible_v<Rollback>)
    : m_rollback(std::move(other.m_rollback))
    , m_active(std::exchange(other.m_active, false)) {}

    ~exception_guard() {
        if (m_active) {
            m_rollback();
        }
    }

    exception_guard& operator=(const exception_guard&) = delete;
    exception_guard& operator=(exception_guard&&) = delete;

    void dismiss() noexcept { m_active = false; }

private:
    rollback_type m_rollback;
    bool m_active;
};

template <class Rollback>
exception_guard<Rollback> make_exception_guard(Rollback rollback) {
    return exception_guard<Rollback>(std::move(rollback));
}

} // namespace utility

// bring symbols into parent namespace

using utility::make_exception_guard;

} // namespace shard
