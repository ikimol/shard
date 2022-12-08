// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_MEMORY_DATA_HPP
#define SHARD_MEMORY_DATA_HPP

#include <string_view>
#include <utility>

namespace shard {
namespace memory {

struct data {
    /// Create empty data
    data() = default;

    /// Create data with the given bytes and size
    data(std::byte* bytes, std::size_t size) : bytes(bytes), size(size) {}

    std::byte* bytes = nullptr;
    std::size_t size = 0;
};

/// Get the contents of the buffer as a string view
std::string_view to_string_view(const data& data) {
    auto c_str = reinterpret_cast<const char*>(data.bytes);
    return std::string_view {c_str, data.size};
}

} // namespace memory

// bring symbols into parent namespace

using memory::data;

} // namespace shard

#endif // SHARD_MEMORY_DATA_HPP
