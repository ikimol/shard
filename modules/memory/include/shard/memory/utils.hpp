// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_MEMORY_UTILS_HPP
#define SHARD_MEMORY_UTILS_HPP

#include <cstddef>
#include <cstdint>

namespace shard {
namespace memory {

/// Cast an address to its unsigned integer representation
inline std::uintptr_t as_uint(const void* ptr) {
    return reinterpret_cast<std::uintptr_t>(ptr);
}

/// Cast an unsigned integer to the address
inline void* as_ptr(std::uintptr_t i) {
    return reinterpret_cast<void*>(i);
}

/// Cast an unsigned integer to the address
inline const void* as_ptr_const(std::uintptr_t i) {
    return reinterpret_cast<const void*>(i);
}

/// Move the pointer \p n bytes forward
inline void* add(void* ptr, std::size_t n) {
    return as_ptr(as_uint(ptr) + n);
}

/// Move the pointer \p n bytes forward
inline const void* add(const void* ptr, std::size_t n) {
    return as_ptr_const(as_uint(ptr) + n);
}

/// Move the pointer \p n bytes backwards
inline void* sub(void* ptr, std::size_t n) {
    return as_ptr(as_uint(ptr) - n);
}

/// Move the pointer \p n bytes backwards
inline const void* sub(const void* ptr, std::size_t n) {
    return as_ptr_const(as_uint(ptr) - n);
}

/// Calculate how many bytes the address would need to be shifted forward to be
/// aligned
inline std::size_t get_padding(const void* ptr, std::size_t align) {
    // assume: align is a power of 2
    // (align - 1) will set the log2(align) least significant bits to 1
    // then using this value in a bitwise AND will result in the misalignment
    // because the most significant bits, that do not affect the alignment, are
    // simply masked out
    // only the log2(align) least significant bits affect the alignment
    auto padding = align - (as_uint(ptr) & (align - 1));
    // already aligned
    if (padding == align) {
        return 0;
    }
    return padding;
}

template <typename T>
std::size_t get_padding(const void* ptr, std::size_t align) {
    if (alignof(T) > align) {
        align = alignof(T);
    }
    return sizeof(T) + get_padding(add(ptr, sizeof(T)), align);
}

/// Check if a memory address is n-byte aligned
inline bool is_aligned(const void* ptr, std::size_t align) {
    return get_padding(ptr, align) == 0;
}

/// Check if an object is aligned
template <typename T>
bool is_aligned(const T* ptr) {
    return is_aligned(ptr, alignof(T));
}

} // namespace memory
} // namespace shard

#endif // SHARD_MEMORY_UTILS_HPP
