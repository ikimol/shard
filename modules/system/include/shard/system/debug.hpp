// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/system/platform.hpp"

// clang-format off

#if defined(NDEBUG)
    #define SHARD_DEBUG_BREAK() ((void) 0)
#else
    #if defined(SHARD_WINDOWS)
        #include <crtdbg.h>
        #define SHARD_DEBUG_BREAK() _CrtDbgBreak()
    #else
        #include <csignal>
        #define SHARD_DEBUG_BREAK() raise(SIGTRAP)
    #endif
#endif

// clang-format on
