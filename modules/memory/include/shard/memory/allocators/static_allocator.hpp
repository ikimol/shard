// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_MEMORY_STATIC_ALLOCATOR_HPP
#define SHARD_MEMORY_STATIC_ALLOCATOR_HPP

#include "shard/memory/allocators/linear_allocator.hpp"

namespace shard {
namespace memory {

template <std::size_t t_size>
class static_allocator : public linear_allocator {
public:
    static_allocator() noexcept
    : linear_allocator(m_buffer, t_size) {}

private:
    char m_buffer[t_size] = {};
};

} // namespace memory

// bring symbols into parent namespace

using memory::static_allocator;

} // namespace shard

#endif // SHARD_MEMORY_STATIC_ALLOCATOR_HPP
