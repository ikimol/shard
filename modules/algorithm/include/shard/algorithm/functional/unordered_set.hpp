// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/algorithm/functional.hpp"

#include <unordered_set>

namespace shard::algorithm::functional::detail {

template <typename... Ts>
struct inserter<std::unordered_set<Ts...>> {
    using container_type = std::unordered_set<Ts...>;

    template <typename... Args>
    static void insert(container_type& c, Args&&... args) {
        c.emplace(std::forward<Args>(args)...);
    }
};

template <typename... Ts>
struct reserver<std::unordered_set<Ts...>> {
    using container_type = std::unordered_set<Ts...>;

    static void reserve(container_type& c, std::size_t new_cap) { c.reserve(new_cap); }
};

} // namespace shard::algorithm::functional::detail
