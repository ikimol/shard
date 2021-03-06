// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_CONCURRENCY_CHANNEL_HPP
#define SHARD_CONCURRENCY_CHANNEL_HPP

#include <shard/core/non_copyable.hpp>
#include <shard/optional.hpp>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

namespace shard {
namespace concurrency {

template <typename T>
class channel : private core::non_copyable {
public:
    using value_type = T;
    using size_type = std::size_t;

public:
    /// Default constructor
    channel() = default;

    /// Destructor that closes the channel and thus notifies waiting threads
    ~channel() { close(); }

    /// Add a new item to the channel by copying it
    void put(const value_type& value) {
        if (!m_open) {
            return;
        }
        // put the value on the queue
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push(value);
        }
        // lock released before notifying
        m_cv.notify_one();
    }

    /// Add a new item to the channel by moving it
    void put(value_type&& value) {
        if (!m_open) {
            return;
        }
        // put the value on the queue
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push(std::move(value));
        }
        // lock released before notifying
        m_cv.notify_one();
    }

    /// Add a new item to the channel by creating it in-place
    template <typename... Args>
    void emplace(Args&&... args) {
        if (!m_open) {
            return;
        }
        // create the value in-place on the queue
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.emplace(std::forward<Args>(args)...);
        }
        // lock released before notifying
        m_cv.notify_one();
    }

    /// Pop and retrieve the next value from the channel
    ///
    /// \note The calling thread is *NOT* blocked.
    ///
    /// \param out The reference to be assigned the value
    ///
    /// \return True if a value was retrieved, false if the channel was closed
    /// or if the queue is empty
    bool try_get(value_type& out) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_open || m_queue.empty()) {
            return false;
        }
        out = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

    /// Pop and retrieve the next value from the channel
    ///
    /// \note The calling thread is *NOT* blocked.
    ///
    /// \return An optional value with the result if a value was retrieved from
    /// the queue, nullopt otherwise
    shard::optional<T> try_get_optional() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_open || m_queue.empty()) {
            return shard::nullopt;
        }
        auto result = make_optional(std::move(m_queue.front()));
        m_queue.pop();
        return result;
    }

    /// Pop and retrieve the next value from the channel
    ///
    /// \note If the queue is empty, the calling thread is blocked until there
    /// is an item on the queue or the channel is closed.
    ///
    /// \param out The reference to be assigned the value
    ///
    /// \return True if a value was retrieved, false if the channel was closed
    bool wait_get(value_type& out) {
        std::unique_lock<std::mutex> lock(m_mutex);
        // unblock if closed or there's something new on the queue
        m_cv.wait(lock, [this] { return !m_open || !m_queue.empty(); });
        if (!m_open) {
            return false;
        }
        out = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

    /// Pop and retrieve the next value from the channel
    ///
    /// \note If the queue is empty, the calling thread is blocked until there
    /// is an item on the queue or the channel is closed.
    ///
    /// \return An optional value with the result if a value was retrieved from
    /// the queue, nullopt otherwise
    shard::optional<T> wait_get_optional() {
        std::unique_lock<std::mutex> lock(m_mutex);
        // unblock if closed or there's something new on the queue
        m_cv.wait(lock, [this] { return !m_open || !m_queue.empty(); });
        if (!m_open) {
            return shard::nullopt;
        }
        auto result = make_optional(std::move(m_queue.front()));
        m_queue.pop();
        return result;
    }

    /// Get number of items on the channel
    size_type size() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    /// Check if channel is empty
    bool is_empty() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    /// Clear the channel, removing everything from the queue
    void clear() {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            while (!m_queue.empty()) {
                m_queue.pop();
            }
        }
        // lock released before notifying
        m_cv.notify_all();
    }

    /// Open the channel, notifying all waiting threads
    ///
    /// This will open the channel and notify threads only if it was closed
    /// before.
    void open() {
        bool was_open = m_open;
        m_open = true;
        if (!was_open) {
            m_cv.notify_all();
        }
    }

    /// Close the channel, notifying all waiting threads
    ///
    /// This will close the channel and notify threads only if it was open
    /// before. The channel can later be reopened using open().
    ///
    /// \note This will *NOT* clear the channel.
    void close() {
        bool was_open = m_open;
        m_open = false;
        if (was_open) {
            m_cv.notify_all();
        }
    }

    /// Check if the channel is open
    bool is_open() const { return m_open; }

    /// Notify all waiting threads
    ///
    /// Useful in cases where the channel is populated before any threads are
    /// spawned. Once the threads are created, the channel can notify them that
    /// there are items to processed.
    ///
    /// \note Will only notify if the channel is open.
    void notify() {
        if (m_open) {
            m_cv.notify_all();
        }
    }

private:
    std::queue<value_type> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_open = ATOMIC_VAR_INIT(true);
};

} // namespace concurrency

// bring symbols into parent namespace

using concurrency::channel;

} // namespace shard

#endif // SHARD_CONCURRENCY_CHANNEL_HPP
