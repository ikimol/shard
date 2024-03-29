// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <atomic>
#include <cassert>
#include <cstddef>
#include <new>
#include <optional>
#include <utility>

namespace shard {
namespace concurrency {

template <typename T, std::size_t Capacity>
class mpsc_queue {
public:
    using value_type = T;
    using size_type = std::size_t;

public:
    mpsc_queue() {
        m_data = new std::byte[sizeof(value_type) * Capacity];
        m_states = new std::atomic<bool>[Capacity];
    }

    /// Add a new item to the queue by copying it
    ///
    /// \note It is safe to call this from multiple threads
    ///
    /// \return true if the item was added, false otherwise
    bool push(const value_type& value) {
        if (is_full()) {
            return false;
        }

        auto head = next_head();
        auto offset = sizeof(value_type) * head;
        auto memory = reinterpret_cast<value_type*>(&m_data[offset]);
        *memory = value;
        m_states[head].store(true, std::memory_order_release);

        return true;
    }

    /// Add a new item to the queue by moving it
    ///
    /// \note It is safe to call this from multiple threads
    ///
    /// \return true if the item was added, false otherwise
    bool push(value_type&& value) {
        if (is_full()) {
            return false;
        }

        auto head = next_head();
        auto offset = sizeof(value_type) * head;
        auto memory = reinterpret_cast<value_type*>(&m_data[offset]);
        *memory = std::move(value);
        m_states[head].store(true, std::memory_order_release);

        return true;
    }

    /// Add a new item to the queue by creating it in-place
    ///
    /// \note It is safe to call this from multiple threads
    ///
    /// \return true if the item was added, false otherwise
    template <typename... Args>
    bool emplace(Args&&... args) {
        if (is_full()) {
            return false;
        }

        auto head = next_head();
        auto offset = sizeof(value_type) * head;
        new (&m_data[offset]) value_type(std::forward<Args>(args)...);
        m_states[head].store(true, std::memory_order_release);

        return true;
    }

    /// Pop and retrieve the next item from the queue
    ///
    /// \warning It is *NOT* safe to call this from multiple threads
    std::optional<value_type> pop() {
        if (!m_states[m_tail].load(std::memory_order_acquire)) {
            return std::nullopt;
        }

        assert(m_states[m_tail]);
        auto offset = sizeof(value_type) * m_tail;
        auto value = std::move(*reinterpret_cast<value_type*>(&m_data[offset]));
        m_states[m_tail].store(false, std::memory_order_release);

        if (++m_tail >= Capacity) {
            m_tail = 0;
        }

        auto old_size = m_size.fetch_sub(1, std::memory_order_release);
        assert(old_size > 0);

        return value;
    }

    bool is_empty() const { return m_size.load(std::memory_order_relaxed) == 0; }

    std::size_t size() const { return m_size.load(std::memory_order_relaxed); }

    std::size_t capacity() const { return Capacity; }

private:
    bool is_full() {
        auto size = m_size.fetch_add(1, std::memory_order_acquire);
        if (size >= Capacity) {
            m_size.fetch_sub(1, std::memory_order_release);
            return true;
        }
        return false;
    }

    size_type next_head() {
        auto head = m_head.fetch_add(1, std::memory_order_acquire);
        if (head >= Capacity) {
            head = 0;
            m_head.store(1, std::memory_order_release);
        }
        assert(!m_states[head]);
        return head;
    }

private:
    std::byte* m_data = nullptr;
    std::atomic<bool>* m_states = ATOMIC_VAR_INIT(nullptr);

    std::atomic<size_type> m_size = ATOMIC_VAR_INIT(0);

    std::atomic<size_type> m_head = ATOMIC_VAR_INIT(0);
    size_type m_tail = 0;
};

} // namespace concurrency

// bring symbols into parent namespace

using concurrency::mpsc_queue;

} // namespace shard
