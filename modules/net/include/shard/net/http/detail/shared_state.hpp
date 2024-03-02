// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/net/http/result.hpp"

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>

namespace shard::net::http {

using success_callback = std::function<void(const response&)>;
using error_callback = std::function<void(const curl::error&)>;

namespace detail {

struct shared_state {
    using ptr = std::shared_ptr<shared_state>;

    void invoke_callback() const {
        if (result.has_value() && on_success) {
            on_success(result.value());
        } else if (on_error) {
            on_error(result.error());
        }
    }

    bool is_available = false;

    success_callback on_success;
    error_callback on_error;
    result result;

    std::mutex mutex;
    std::condition_variable cv;
};

} // namespace detail
} // namespace shard::net::http
