// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/net/http/detail/shared_state.hpp"
#include "shard/net/http/response.hpp"

#include <mutex>

namespace shard::net::http {

class future {
    friend class client;

public:
    /// Cancel the ongoing request
    void cancel() const {
        std::lock_guard lock(m_state->mutex);
        m_state->is_cancelled = true;
    }

    /// Wait for the result to be available
    void wait() const {
        std::unique_lock lock(m_state->mutex);
        // unblock when the state has the result
        m_state->cv.wait(lock, [this] { return m_state->is_available; });
    }

    /// Get the result by blocking the calling thread until available
    [[nodiscard]] http::result&& get() {
        std::unique_lock lock(m_state->mutex);
        // unblock when the state has the result
        m_state->cv.wait(lock, [this] { return m_state->is_available; });
        m_state->is_available = false;
        return std::move(m_state->result);
    }

    /// Execute the callback with the response if the request succeeded
    ///
    /// \note This does not block the calling thread
    void on_success(success_callback callback) {
        std::lock_guard lock(m_state->mutex);
        m_state->on_success = std::move(callback);
    }

    /// Execute the callback with the error if the request failed
    ///
    /// \note This does not block the calling thread
    void on_error(error_callback callback) {
        std::lock_guard lock(m_state->mutex);
        m_state->on_error = std::move(callback);
    }

private:
    explicit future(detail::shared_state::ptr&& state)
    : m_state(std::move(state)) {}

private:
    detail::shared_state::ptr m_state;
};

} // namespace shard::net::http
