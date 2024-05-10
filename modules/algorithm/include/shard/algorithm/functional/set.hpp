// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/algorithm/functional.hpp"

#include <set>

namespace shard::algorithm::functional::detail {

template <typename... Ts>
struct inserter<std::set<Ts...>> {
    using container_type = std::set<Ts...>;

    template <typename... Args>
    static void insert(container_type& c, Args&&... args) {
        c.emplace(std::forward<Args>(args)...);
    }
};

} // namespace shard::algorithm::functional::detail
