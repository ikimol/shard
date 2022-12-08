// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_SYSTEM_PLATFORM_HPP
#define SHARD_SYSTEM_PLATFORM_HPP

// clang-format off
#if defined(__ANDROID__)
    #define SHARD_ANDROID
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
    #define SHARD_UNIX
    #define SHARD_LINUX
#elif defined(__APPLE__)
    #define SHARD_UNIX
    #include <TargetConditionals.h>
    #if defined(TARGET_OS_MAC)
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

#endif // SHARD_SYSTEM_PLATFORM_HPP
