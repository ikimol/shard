// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/system/env.hpp>
#include <shard/system/platform.hpp>

#include <iostream>

namespace {

#if defined(SHARD_UNIX)
constexpr auto g_home_var = "HOME";
#elif defined(SHARD_WINDOWS)
constexpr auto g_home_var = "HOMEPATH";
#endif

} // namespace

int main(int /* argc */, char* /* argv */[]) {
    if (auto home = shard::env::get(g_home_var)) {
        std::cout << "home directory: " << *home << '\n';
    }

    if (!shard::env::set("FOO", "42")) {
        return 1;
    }

    if (!shard::env::has("FOO")) {
        return 1;
    }

    std::string old_foo;
    if (shard::env::set("FOO", "0", &old_foo)) {
        std::cout << "old foo: " << old_foo << '\n';
    }

    if (!shard::env::unset("foo")) {
        return 1;
    }

    if (shard::env::has("foo")) {
        return 1;
    }

    return 0;
}