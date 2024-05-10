// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/algorithm/functional.hpp"

#include <queue>

namespace shard::algorithm::functional::detail {

template <typename... Ts>
struct inserter<std::queue<Ts...>> {
    using container_type = std::queue<Ts...>;

    template <typename... Args>
    static void insert(container_type& c, Args&&... args) {
        c.emplace(std::forward<Args>(args)...);
    }
};

template <typename... Ts>
struct inserter<std::priority_queue<Ts...>> {
    using container_type = std::priority_queue<Ts...>;

    template <typename... Args>
    static void insert(container_type& c, Args&&... args) {
        c.emplace(std::forward<Args>(args)...);
    }
};

} // namespace shard::algorithm::functional::detail
