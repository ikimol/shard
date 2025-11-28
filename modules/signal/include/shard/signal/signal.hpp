// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/signal/connection.hpp"
#include "shard/signal/slot.hpp"

#include <vector>

namespace shard {
namespace detail {

class slot_base;

class signal_base {
    friend class shard::connection;

public:
    virtual ~signal_base() = default;

protected:
    virtual void disconnect(const std::shared_ptr<slot_base>& slot) = 0;
};

} // namespace detail

/// Represents an event that slots can be connected to
template <typename... Args>
class signal final : public detail::signal_base {
private:
    using slot_type = slot<Args...>;

public:
    signal() = default;

    /// Destructor
    ~signal() override = default;

    signal(const signal&) = delete;
    signal(signal&&) = delete;

    signal& operator=(const signal&) = delete;
    signal& operator=(signal&&) = delete;

    /// Get the number of connected slots
    std::size_t slot_count() const { return m_slots.size(); }

    /// Connect a function object as a slot
    template <typename Function>
    [[nodiscard]] connection connect(Function&& fn) {
        std::shared_ptr<slot_type> slot;
        slot = std::make_shared<slot_type>(std::forward<Function>(fn));
        m_slots.push_back(std::move(slot));
        return connection {this, m_slots.back()};
    }

    /// Emit the signal calling every enabled slot
    template <typename... CallArgs>
    void emit(CallArgs&&... args) {
        for (const auto& slot : m_slots) {
            slot->invoke(std::forward<CallArgs>(args)...);
        }
    }

    /// Emit the signal calling every enabled slot
    template <typename... CallArgs>
    void operator()(CallArgs&&... args) {
        emit(std::forward<CallArgs>(args)...);
    }

protected:
    void disconnect(const std::shared_ptr<detail::slot_base>& slot) override {
        m_slots.erase(std::remove(m_slots.begin(), m_slots.end(), slot), m_slots.end());
    }

private:
    std::vector<std::shared_ptr<slot_type>> m_slots;
};

// implementation

inline bool connection::is_enabled() const {
    if (auto slot = m_slot.lock()) {
        return slot->is_enabled();
    }
    return false;
}

inline void connection::set_enabled(bool enabled) {
    if (auto slot = m_slot.lock()) {
        slot->set_enabled(enabled);
    }
}

inline void connection::disconnect() {
    if (auto slot = m_slot.lock()) {
        m_signal->disconnect(slot);
    }
}

} // namespace shard
