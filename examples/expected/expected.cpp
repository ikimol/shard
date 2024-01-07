// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include <shard/expected.hpp>

#include <iostream>

enum class error : int {
    some_error = -1,
};

static shard::expected<int, error> succeed() {
    return 42;
}

static shard::expected<int, error> fail() {
    return shard::unexpected(error::some_error);
}

int main(int /* argc */, char* /* argv */[]) {
    std::array methods = {succeed, fail};

    std::srand(std::time(nullptr)); /* NOLINT */
    auto index = std::rand() % 2;   /* NOLINT */

    if (auto result = methods[index]()) {
        std::cout << "result: " << *result << '\n';
    } else {
        std::cerr << "error: " << static_cast<int>(result.error()) << '\n';
    }
    return 0;
}