// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/system/env.hpp>

#include <iostream>

int main(int /* argc */, char* /* argv */[]) {
    if (auto home = shard::env::get("HOME")) {
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