// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

namespace shard {
namespace bit {

enum class endian {
#if defined(_MSC_VER) && !defined(__clang__)
    little = 0,
    big = 1,
    native = little
#else
    little = __ORDER_LITTLE_ENDIAN__,
    big = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#endif
};

} // namespace bit

// bring symbols into parent namespace

using bit::endian;

} // namespace shard
