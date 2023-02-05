// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_SIGNAL_HPP
#define SHARD_SIGNAL_HPP

#include <shard/meta/type_traits.hpp>

#include <algorithm>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace shard {

class signal_base;
class slot_base;

class connection {
public:
    connection() = default;

    /// Copy constructor
    connection(const connection& other) = default;

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

    /// Copy assignment operator
    connection& operator=(const connection& other) = default;

    /// Move assignment operator
    connection& operator=(connection&& other) noexcept {
        m_signal = other.m_signal;
        m_slot = std::move(other.m_slot);
        other.m_signal = nullptr;
        other.m_slot.reset();
        return *this;
    }

    /// Check if this connection is correctly hooked up
    bool is_valid() { return m_signal && !m_slot.expired(); }

    /// Disconnect the underlying slot from its signal
    void disconnect();

    /// Check if the slot is enabled
    bool is_enabled() const;

    /// Temporarily enable or disable the slot
    void set_enabled(bool enabled);

    /// Convenience function to disable the slot
    void disable() { set_enabled(false); }

    /// Convenience function to enable the slot
    void enable() { set_enabled(true); }

private:
    signal_base* m_signal = nullptr;
    std::weak_ptr<slot_base> m_slot;
};

class scoped_connection {
public:
    scoped_connection() = default;

    /// Implicit constructor from a non-scoped rvalue connection
    scoped_connection(connection&& connection) /* NOLINT */
    : m_connection(std::move(connection)) {}

    /// Assignment operator from a non-scoped connection
    scoped_connection& operator=(connection&& other) noexcept {
        m_connection = std::move(other);
        return *this;
    }

    /// Destructor
    ~scoped_connection() { m_connection.disconnect(); }

private:
    connection m_connection;
};

class slot_base {
public:
    slot_base() = default;

    virtual ~slot_base() = default;

    /// Disabled copy constructor
    slot_base(const slot_base& other) = delete;

    /// Disabled copy assignment operator
    slot_base& operator=(const slot_base& other) = delete;

public:
    bool enabled = true;
};

template <typename... Args>
class slot final : public slot_base {
public:
    using function_type = std::function<void(Args...)>;

public:
    /// Construct a slot with a function pointer
    explicit slot(function_type function)
    : m_function(std::move(function)) {}

    /// Invoke the function pointer
    template <typename... CallArgs>
    void operator()(CallArgs&&... args) {
        if (enabled && m_function) {
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
public:
    using slot_type = slot<Args...>;
    using slot_type_noarg = slot<>;

    using function_type = typename slot_type::function_type;
    using function_type_noarg = typename slot_type_noarg::function_type;

    // clang-format off
    template <typename T> using member_function_type = void (T::*)(Args...);
    template <typename T> using member_function_type_noarg = void (T::*)();
    // clang-format on

public:
    ~signal() { disconnect_all(); }

    /// Get the number of connected slots
    std::size_t slot_count() const { return m_slots.size(); }

    /// Connect a function object as a slot
    connection connect(function_type function) {
        m_slots.push_back(std::make_shared<slot_type>(std::move(function)));
        return {this, m_slots.back()};
    }

    /// Connect a function object taking no arguments as a slot
    template <meta::disable_if_t<meta::is_empty<Args...>::value>* = nullptr>
    connection connect(function_type_noarg function) /* NOLINT */ {
        auto wrapper = [=](Args&&...) -> void {
            function();
        };
        m_slots.push_back(std::make_shared<slot_type>(wrapper));
        return {this, m_slots.back()};
    }

    /// Connect a member function as a slot
    template <typename T>
    connection connect(member_function_type<T> member_function, T* object) {
        auto wrapper = [=](Args&&... args) -> void {
            (object->*member_function)(std::forward<Args>(args)...);
        };
        m_slots.push_back(std::make_shared<slot_type>(wrapper));
        return {this, m_slots.back()};
    }

    /// Connect a member function taking no arguments as a slot
    template <typename T, meta::disable_if_t<meta::is_empty<Args...>::value>* = nullptr>
    connection connect(member_function_type_noarg<T> member_function, T* object) {
        auto wrapper = [=](Args&&...) -> void {
            (object->*member_function)();
        };
        m_slots.push_back(std::make_shared<slot_type>(wrapper));
        return {this, m_slots.back()};
    }

    /// Disconnect every slot
    void disconnect_all() { m_slots.clear(); }

    /// Emit the signal calling every enabled slot
    template <typename... CallArgs>
    void emit(CallArgs&&... args) {
        for (const auto& slot : m_slots) {
            if (slot) {
                (*slot)(std::forward<CallArgs>(args)...);
            }
        }
    }

    /// Call operator to emit the signal
    template <typename... CallArgs>
    void operator()(CallArgs&&... args) {
        emit(std::forward<CallArgs>(args)...);
    }

protected:
    void disconnect(const std::shared_ptr<slot_base>& slot) final {
        auto it = std::find(m_slots.begin(), m_slots.end(), slot);
        if (it != m_slots.end()) {
            m_slots.erase(it);
        }
    }

private:
    std::vector<std::shared_ptr<slot_type>> m_slots;
};

inline void connection::disconnect() {
    if (auto slot = m_slot.lock()) {
        m_signal->disconnect(slot);
    }
}

inline bool connection::is_enabled() const {
    if (auto slot = m_slot.lock()) {
        return (*slot).enabled;
    }
    return false;
}

inline void connection::set_enabled(bool enabled) {
    if (auto slot = m_slot.lock()) {
        (*slot).enabled = enabled;
    }
}

} // namespace shard

#ifndef signals
#define signals public
#endif

#endif // SHARD_SIGNAL_HPP
