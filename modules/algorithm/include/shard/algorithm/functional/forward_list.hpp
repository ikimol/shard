// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/algorithm/functional.hpp"

#include <forward_list>

namespace shard::algorithm::functional::detail {

template <typename... Ts>
struct inserter<std::forward_list<Ts...>> {
    using container_type = std::forward_list<Ts...>;

    template <typename... Args>
    static void insert(container_type& c, Args&&... args) {
        c.emplace_front(std::forward<Args>(args)...);
    }
};

} // namespace shard::algorithm::functional::detail
