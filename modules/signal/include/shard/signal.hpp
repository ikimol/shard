// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <type_traits>
#include <vector>

namespace shard {

class signal_base;
class slot_base;

class connection {
public:
    /// Default constructor
    connection() = default;

    /// Move constructor
    connection(connection&& other) noexcept
    : m_signal(other.m_signal)
    , m_slot(std::move(other.m_slot)) {
        other.m_signal = nullptr;
        other.m_slot.reset();
    }

    /// Construct a connection from a signal and a slot
    connection(signal_base* signal, const std::shared_ptr<slot_base>& slot)
    : m_signal(signal)
    , m_slot(slot) {}

    /// Move assignment operator
    connection& operator=(connection&& other) noexcept {
        m_signal = other.m_signal;
        m_slot = std::move(other.m_slot);
        other.m_signal = nullptr;
        other.m_slot.reset();
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
    signal_base* m_signal = nullptr;
    std::weak_ptr<slot_base> m_slot;
};

class slot_base {
public:
    // check if the slot is enabled
    bool is_enabled() const { return m_enabled; }

    // set if the slot should be enabled
    void set_enabled(bool enabled) { m_enabled = enabled; }

private:
    bool m_enabled = true;
};

template <typename... Args>
class slot final : public slot_base {
public:
    using function_type = std::function<void(Args...)>;

public:
    // construct a slot with a function pointer
    explicit slot(function_type function)
    : m_function(std::move(function)) {}

    // invoke the stored function
    template <typename... CallArgs>
    void invoke(CallArgs&&... args) {
        if (is_enabled()) {
            m_function(std::forward<CallArgs>(args)...);
        }
    }

private:
    function_type m_function;
};

class signal_base {
    friend class connection;

public:
    virtual ~signal_base() = default;

protected:
    virtual void disconnect(const std::shared_ptr<slot_base>& slot) = 0;
};

template <typename... Args>
class signal final : public signal_base {
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
        if constexpr (std::is_invocable_v<Function, Args...>) {
            slot = std::make_shared<slot_type>(std::forward<Function>(fn));
        } else {
            slot = std::make_shared<slot_type>([fn = std::forward<std::decay_t<Function>>(fn)](auto&&...) { fn(); });
        }
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
    void disconnect(const std::shared_ptr<slot_base>& slot) override {
        m_slots.erase(std::remove(m_slots.begin(), m_slots.end(), slot), m_slots.end());
    }

private:
    std::vector<std::shared_ptr<slot_type>> m_slots;
};

// helpers

template <class T, typename... Args>
std::function<void(Args...)> bind(T* object, void (T::*method)(Args...)) {
    return [object, method](Args&&... args) {
        return (object->*method)(std::forward<Args>(args)...);
    };
}

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
