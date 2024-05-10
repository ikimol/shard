// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/algorithm/functional.hpp"

#include <deque>

namespace shard::algorithm::functional::detail {

template <typename... Ts>
struct inserter<std::deque<Ts...>> {
    using container_type = std::deque<Ts...>;

    template <typename... Args>
    static void insert(container_type& c, Args&&... args) {
        c.emplace_back(std::forward<Args>(args)...);
    }
};

} // namespace shard::algorithm::functional::detail