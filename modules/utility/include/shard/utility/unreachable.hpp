// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <cassert>

namespace shard {

/// Mark a code path to the compiler as unreachable
[[noreturn]] inline void unreachable() {
#if defined(_MSC_VER) && !defined(__clang__)
    __assume(false);
#elif defined(__has_builtin) && __has_builtin(__builtin_unreachable)
    __builtin_unreachable();
#else
    assert(false);
#endif
}

} // namespace shard
