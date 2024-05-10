// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/algorithm/functional.hpp"

#include <vector>

namespace shard::algorithm::functional::detail {

template <typename... Ts>
struct inserter<std::vector<Ts...>> {
    using container_type = std::vector<Ts...>;

    template <typename... Args>
    static void insert(container_type& c, Args&&... args) {
        c.emplace_back(std::forward<Args>(args)...);
    }
};

template <typename... Ts>
struct reserver<std::vector<Ts...>> {
    using container_type = std::vector<Ts...>;

    static void reserve(container_type& c, std::size_t new_cap) { c.reserve(new_cap); }
};

} // namespace shard::algorithm::functional::detail
