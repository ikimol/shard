// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_NET_HTTP_FUTURE_HPP
#define SHARD_NET_HTTP_FUTURE_HPP

#include "shard/net/http/detail/shared_state.hpp"
#include "shard/net/http/response.hpp"

#include <mutex>

namespace shard::net::http {

class future {
    friend class client;

public:
    /// Get the response by blocking the calling thread until available
    const http::response& get() {
        std::unique_lock<std::mutex> lock(m_state->mutex);
        // unblock when the state has the response
        m_state->cv.wait(lock, [this] { return m_state->response != nullptr; });
        return *(m_state->response);
    }

    /// Execute a function once the response is available
    ///
    /// \note This does not block the calling thread
    void then(response_callback callback) {
        std::lock_guard<std::mutex> lock(m_state->mutex);
        m_state->callback = std::move(callback);
    }

private:
    explicit future(detail::shared_state::ptr&& state)
    : m_state(std::move(state)) {}

private:
    detail::shared_state::ptr m_state;
};

} // namespace shard::net::http

#endif // SHARD_NET_HTTP_FUTURE_HPP
