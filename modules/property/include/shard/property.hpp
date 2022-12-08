// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_PROPERTY_HPP
#define SHARD_PROPERTY_HPP

#include <functional>
#include <unordered_set>
#include <utility>

namespace shard {

class property_base;

namespace detail {

inline property_base** current_property() {
    static property_base* property = nullptr;
    return &property;
}

} // namespace detail

class property_base {
public:
    property_base() = default;

    property_base(const property_base& other) : m_dependencies(other.m_dependencies) {
        for (auto property : m_dependencies) {
            property->m_subscribers.insert(this);
        }
    }

    virtual ~property_base() {
        remove_subscribers();
        remove_dependencies();
    }

protected:
    virtual void evaluate() = 0;

    // called to evaluate subscribers when this property is changed
    virtual void notify() {
        auto subscribers = m_subscribers;
        for (auto property : subscribers) {
            property->evaluate();
        }
    }

    // called to register dependencies when this property is accessed
    void accessed() {
        auto current = *detail::current_property();
        if (current && current != this) {
            m_subscribers.insert(current);
            current->m_dependencies.insert(this);
        }
    }

    void remove_subscribers() {
        for (auto property : m_subscribers) {
            property->m_dependencies.erase(this);
        }
        m_subscribers.clear();
    }

    void remove_dependencies() {
        for (auto property : m_dependencies) {
            property->m_subscribers.erase(this);
        }
        m_dependencies.clear();
    }

protected:
    struct evaluation_scope {
        explicit evaluation_scope(property_base* property) : previous(*detail::current_property()) {
            *detail::current_property() = property;
        }

        ~evaluation_scope() { *detail::current_property() = previous; }

        property_base* previous;
    };

private:
    std::unordered_set<property_base*> m_subscribers;  // properties that are updated by this property
    std::unordered_set<property_base*> m_dependencies; // properties that trigger an update on this property
};

template <typename T>
class property : public property_base {
public:
    using value_type = T;
    using binding_type = std::function<T()>;

public:
    /// Default constructor
    property() = default;

    /// Value constructor
    property(const T& value) /* NOLINT */ : m_value(value) {}

    /// Binding constructor
    property(const binding_type& binding) /* NOLINT */ : m_binding(binding) { property<T>::evaluate(); }

    /// Converting value constructor
    template <typename U, std::enable_if_t<std::is_constructible<T, U>::value>* = nullptr>
    property(const U& value) /* NOLINT */ : property(T(value)) {}

    /// Converting binding constructor
    template <typename B,
    std::enable_if_t<std::is_constructible<binding_type, B>::value && !std::is_constructible<T, B>::value>* = nullptr>
    property(const B& binding) /* NOLINT */ : property(binding_type(binding)) {}

    /// Value assignment operator
    property& operator=(const T& value) {
        m_value = value;
        remove_dependencies();
        notify();
        return *this;
    }

    /// Binding assignment operator
    property& operator=(const binding_type& binding) {
        m_binding = binding;
        evaluate();
        return *this;
    }

    /// Converting value assignment operator
    template <typename U, std::enable_if_t<std::is_constructible<T, U>::value>* = nullptr>
    property& operator=(const U& value) {
        *this = T(value);
        return *this;
    }

    /// Converting binding assignment operator
    template <typename B,
    std::enable_if_t<std::is_constructible<binding_type, B>::value && !std::is_constructible<T, B>::value>* = nullptr>
    property& operator=(const B& binding) {
        *this = binding_type(binding);
        return *this;
    }

    /// Get the value of the property
    const T& value() const {
        const_cast<property*>(this)->accessed();
        return m_value;
    }

    /// Value conversion operator
    operator const T&() const /* NOLINT */ { return value(); }

    /// Member access operator
    constexpr const T* operator->() const { return &value(); }

    /// Indirection operator
    constexpr const T& operator*() const { return value(); }

protected:
    // evaluate the value of the property
    void evaluate() override {
        if (m_binding) {
            remove_dependencies();
            evaluation_scope scope(this);
            m_value = m_binding();
        }
        notify();
    }

private:
    value_type m_value;
    binding_type m_binding;
};

template <typename T>
class observed_property : public property<T> {
public:
    using value_type = typename property<T>::value_type;
    using binding_type = typename property<T>::binding_type;

    using callback_type = std::function<void()>;

public:
    observed_property(callback_type callback) /* NOLINT */ : m_callback(std::move(callback)) {}

    observed_property(callback_type callback, const T& value) : property<T>(value), m_callback(std::move(callback)) {}

    observed_property(callback_type callback, binding_type binding) :
    property<T>(binding),
    m_callback(std::move(callback)) {}

    using property<T>::operator=;

protected:
    void notify() override {
        property<T>::notify();
        m_callback();
    }

private:
    callback_type m_callback;
};

} // namespace shard

#endif // SHARD_PROPERTY_HPP
