// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <cstddef>

namespace shard {
namespace memory {

class data {
public:
    /// Create an empty data
    data() = default;

    /// Create a wrapper for data with the given bytes and size
    data(const void* bytes, std::size_t size)
    : m_bytes(bytes)
    , m_size(size) {}

    /// Get the bytes
    const std::byte* bytes() const { return static_cast<const std::byte*>(m_bytes); }

    /// Get the number of bytes
    std::size_t size() const { return m_size; }

private:
    const void* m_bytes = nullptr;
    std::size_t m_size = 0;
};

} // namespace memory

// bring symbols into parent namespace

using memory::data;

} // namespace shard
