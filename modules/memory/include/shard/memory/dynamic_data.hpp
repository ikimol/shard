// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <memory>
#include <utility>

namespace shard {
namespace memory {

class dynamic_data {
public:
    /// Create a wrapper without allocating any memory
    dynamic_data() noexcept
    : m_size(0)
    , m_allocation(nullptr, std::free) {}

    /// Allocate memory of the given size
    explicit dynamic_data(std::size_t size)
    : m_size(size)
    , m_allocation(static_cast<std::byte*>(std::malloc(size)), std::free) {}

    /// Get the bytes
    std::byte* bytes() const noexcept { return m_allocation.get(); }

    /// Get the number of bytes
    std::size_t size() const noexcept { return m_size; }

    /// Stop managing the allocated memory
    std::byte* release() noexcept {
        m_size = 0;
        return m_allocation.release();
    }

    /// Reallocate the memory to the given size
    std::byte* reallocate(std::size_t new_size) {
        auto bytes = m_allocation.release();

        if (auto p = std::realloc(bytes, new_size)) {
            m_allocation.reset(static_cast<std::byte*>(p));
            m_size = new_size;
            return m_allocation.get();
        }

        m_allocation.reset(bytes);

        throw std::bad_alloc();
    }

private:
    std::size_t m_size;
    std::unique_ptr<std::byte, void (*)(void*)> m_allocation;
};

} // namespace memory

// bring symbols into parent namespace

using memory::dynamic_data;

} // namespace shard
