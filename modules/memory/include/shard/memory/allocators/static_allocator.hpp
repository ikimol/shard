// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/memory/allocators/linear_allocator.hpp"

namespace shard {
namespace memory {

template <std::size_t Size>
class static_allocator : public linear_allocator {
public:
    static_allocator() noexcept
    : linear_allocator(m_buffer, Size) {}

private:
    char m_buffer[Size] = {};
};

} // namespace memory

// bring symbols into parent namespace

using memory::static_allocator;

} // namespace shard
