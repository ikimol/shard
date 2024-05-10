// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/algorithm/functional.hpp"

#include <string>

namespace shard::algorithm::functional::detail {

template <typename... Ts>
struct inserter<std::basic_string<Ts...>> {
    using container_type = std::basic_string<Ts...>;

    template <typename... Args>
    static void insert(container_type& c, Args&&... args) {
        c.append(std::forward<Args>(args)...);
    }
};

template <typename... Ts>
struct reserver<std::basic_string<Ts...>> {
    using container_type = std::basic_string<Ts...>;

    static void reserve(container_type& c, std::size_t new_cap) { c.reserve(new_cap); }
};

} // namespace shard::algorithm::functional::detail
