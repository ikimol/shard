// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

// clang-format off
#if defined(__ANDROID__)
    #define SHARD_ANDROID
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
    #define SHARD_UNIX
    #define SHARD_LINUX
#elif defined(__APPLE__)
    #define SHARD_UNIX
    #include <TargetConditionals.h>
    #if defined(TARGET_OS_OSX)
        #define SHARD_MACOS
    #elif defined(TARGET_OS_IOS)
        #define SHARD_IOS
    #endif
#elif defined(unix) || defined(__unix) || defined(__unix__)
    #define SHARD_UNIX
#elif defined(_WIN32) || defined(_WIN64)
	#define SHARD_WINDOWS
#else
    #error "unknown operating system"
#endif
// clang-format on

namespace shard::system {

enum platform_t {
    platform_linux,
    platform_android,
    platform_macos,
    platform_ios,
    platform_windows,
};

/// Get the current platform
platform_t platform();

/// Get the basic name of the current platform
const char* platform_name();

} // namespace shard::system
