// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include <memory>
#include <utility>

namespace shard {
namespace detail {

class signal_base;
class slot_base;

} // namespace detail

/// Represents the connection of a slot to its signal
class connection {
public:
    /// Default constructor
    connection() = default;

    /// Construct a connection from a signal and a slot
    connection(detail::signal_base* signal, std::weak_ptr<detail::slot_base> slot)
    : m_signal(signal)
    , m_slot(std::move(slot)) {}

    /// Move constructor
    connection(connection&& other) noexcept
    : m_signal(std::exchange(other.m_signal, nullptr))
    , m_slot(std::exchange(other.m_slot, std::weak_ptr<detail::slot_base>())) {}

    /// Move assignment operator
    connection& operator=(connection&& other) noexcept {
        if (this != &other) {
            m_signal = std::exchange(other.m_signal, nullptr);
            m_slot = std::exchange(other.m_slot, std::weak_ptr<detail::slot_base>());
        }
        return *this;
    }

    /// Destroy the connection disconnecting the underlying slot from its signal
    ~connection() { disconnect(); }

    /// Check if the connection is alive
    bool is_alive() const { return !m_slot.expired(); }

    /// Check if the slot is enabled
    bool is_enabled() const;

    /// Temporarily enable or disable the slot
    void set_enabled(bool enabled);

    /// Disconnect the underlying slot from its signal
    void disconnect();

private:
    detail::signal_base* m_signal = nullptr;
    std::weak_ptr<detail::slot_base> m_slot;
};

} // namespace shard
