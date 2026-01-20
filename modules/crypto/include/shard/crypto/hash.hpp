// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/memory/data.hpp>
#include <shard/memory/dynamic_data.hpp>

#include <string>

namespace shard::crypto {

/// Represents the result of a hash algorithm
class hash {
    friend hash md5(const data&);
    friend hash sha1(const data&);
    friend hash sha256(const data&);

public:
    enum class algorithm {
        md5,    ///< MD5 algorithm
        sha1,   ///< SHA-1 algorithm
        sha256, ///< SHA-256 algorithm
    };

public:
    /// Get the bytes
    data digest() const { return m_digest; }

    /// Convert the bytes to a hexadecimal string
    std::string to_string() const;

private:
    explicit hash(algorithm algorithm, const data& input);

private:
    dynamic_data m_digest;
};

/// Convenience function type alias
using hash_function_t = hash (*)(const data&);

} // namespace shard::crypto
