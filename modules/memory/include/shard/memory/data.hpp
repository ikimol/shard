// Copyright (c) 2022 Miklos Molnar. All rights reserved.

#ifndef SHARD_MEMORY_DATA_HPP
#define SHARD_MEMORY_DATA_HPP

#include <shard/utility/byte.hpp>

#include <string>
#include <vector>

namespace shard {
namespace memory {

class data {
public:
    using buffer_type = std::vector<shard::byte>;
    using size_type = buffer_type::size_type;

public:
    data() = default;

    explicit data(std::size_t size) : m_buffer(size) {}

    data(const shard::byte* data, std::size_t size) : m_buffer(data, data + size) {}

    /// Get the number of bytes in the buffer
    size_type size() const { return m_buffer.size(); }

    /// Check if the buffer is empty
    bool is_empty() const { return m_buffer.empty(); }

    // access

    /// Get the raw bytes
    shard::byte* bytes() { return m_buffer.data(); }

    /// Get the byte array
    buffer_type& buffer() { return m_buffer; }

    /// Get the byte array
    const buffer_type& buffer() const { return m_buffer; }

    /// Get the contents of the byte array as a string
    std::string string() const {
        auto c_str = reinterpret_cast<const unsigned char*>(m_buffer.data());
        return {c_str, c_str + m_buffer.size()};
    }

private:
    buffer_type m_buffer;
};

} // namespace memory

// bring symbols into parent namespace

using memory::data;

} // namespace shard

#endif // SHARD_MEMORY_DATA_HPP
