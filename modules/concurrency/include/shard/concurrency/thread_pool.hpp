// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_CONCURRENCY_THREAD_POOL_HPP
#define SHARD_CONCURRENCY_THREAD_POOL_HPP

#include "shard/concurrency/channel.hpp"

#include <algorithm>
#include <functional>
#include <optional>
#include <thread>
#include <vector>

namespace shard {
namespace concurrency {

class thread_pool {
public:
    using task_type = std::function<void()>;

public:
    /// Create a thread pool with the maximum number of physical threads
    thread_pool()
    : thread_pool(thread_pool::max_thread_count()) {}

    /// Create a thread pool with the given number of threads
    explicit thread_pool(unsigned int count) {
        try {
            m_threads.reserve(count);
            for (auto i = 0u; i < count; ++i) {
                m_threads.emplace_back(&thread_pool::worker_thread, this);
            }
        } catch (...) {
            destroy();
            throw;
        }
    }

    ~thread_pool() { destroy(); }

    /// Add a new task by copying it
    void run(const task_type& task) { m_tasks.push(task); }

    /// Add a new task by moving it
    void run(task_type&& task) { m_tasks.push(std::move(task)); }

    /// Add a new task by binding the given args
    template <typename F, typename... Args>
    void run(F&& f, Args&&... args) {
        auto bound = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        m_tasks.push(bound);
    }

    /// Stop every thread by closing the channel
    void stop() { m_tasks.close(); }

    /// Get the number of threads in the pool
    std::size_t thread_count() const { return m_threads.size(); }

    /// Get the number of tasks waiting to be executed
    std::size_t task_count() const { return m_tasks.size(); }

public:
    /// Get the maximum number of physical threads
    static unsigned int max_thread_count() noexcept { return std::max(std::thread::hardware_concurrency(), 2u) - 1u; }

private:
    // thread function polling and executing the tasks
    void worker_thread() {
        while (auto task = m_tasks.pop()) {
            (*task)();
        }
    }

    // clear the channel and join the threads
    void destroy() {
        m_tasks.close();
        for (auto& thread : m_threads) {
            thread.join();
        }
        m_threads.clear();
        m_tasks.clear();
    }

private:
    std::vector<std::thread> m_threads;
    channel<task_type> m_tasks;
};

} // namespace concurrency

// bring symbols into parent namespace

using concurrency::thread_pool;

} // namespace shard

#endif // SHARD_CONCURRENCY_THREAD_POOL_HPP
