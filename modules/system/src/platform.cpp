// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include "shard/system/platform.hpp"

static constexpr const char* g_os_names[] = {"linux", "android", "macos", "ios", "windows"};

namespace shard::system {

platform_t platform() {
#if defined(SHARD_ANDROID)
    return platform_t::platform_android;
#elif defined(SHARD_IOS)
    return platform_t::platform_ios;
#elif defined(SHARD_LINUX)
    return platform_t::platform_linux;
#elif defined(SHARD_MACOS)
    return platform_t::platform_macos;
#elif defined(SHARD_WINDOWS)
    return platform_t::platform_windows;
#else
#error "Unsupported platform"
#endif
}

const char* platform_name() {
    auto i = static_cast<unsigned char>(platform());
    return g_os_names[i];
}

} // namespace shard::system
