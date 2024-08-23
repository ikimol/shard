// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <cstdlib>
#include <memory>

namespace shard {
namespace memory {

class dynamic_data {
public:
    /// Create a wrapper without allocating any memory
    dynamic_data() noexcept
    : m_size(0) {}

    /// Allocate memory of the given size
    explicit dynamic_data(std::size_t size)
    : m_size(size)
    , m_allocation(std::malloc(size)) {}

    /// Get the bytes
    std::byte* bytes() const noexcept { return static_cast<std::byte*>(m_allocation.get()); }

    /// Get the number of bytes
    std::size_t size() const noexcept { return m_size; }

    /// Stop managing the allocated memory
    std::byte* release() noexcept {
        m_size = 0;
        return static_cast<std::byte*>(m_allocation.release());
    }

    /// Reallocate the memory to the given size
    std::byte* reallocate(std::size_t new_size) {
        if (auto p = std::realloc(m_allocation.get(), new_size)) /* NOLINT */ {
            m_allocation.release(); /* NOLINT */
            m_allocation.reset(static_cast<std::byte*>(p));
            m_size = new_size;
            return static_cast<std::byte*>(m_allocation.get());
        }

        throw std::bad_alloc();
    }

private:
    struct free_deleter {
        void operator()(void* ptr) const { std::free(ptr); }
    };

private:
    std::size_t m_size;
    std::unique_ptr<void, free_deleter> m_allocation;
};

} // namespace memory

// bring symbols into parent namespace

using memory::dynamic_data;

} // namespace shard
