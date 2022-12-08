// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_HTTP_DETAIL_SHARED_STATE_HPP
#define SHARD_HTTP_DETAIL_SHARED_STATE_HPP

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>

namespace shard::net::http {

class response;

using response_callback = std::function<void(const response&)>;

namespace detail {

struct shared_state {
    using ptr = std::shared_ptr<shared_state>;

    response_callback callback;
    std::unique_ptr<response> response;

    std::mutex mutex;
    std::condition_variable cv;
};

} // namespace detail
} // namespace shard::net::http

#endif // SHARD_HTTP_DETAIL_SHARED_STATE_HPP
