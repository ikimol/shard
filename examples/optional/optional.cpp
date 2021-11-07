// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/optional.hpp>

#include <iostream>

#if __cplusplus >= 201703L
#include <optional>
#endif

inline shard::optional<int> to_int(const std::string& str) {
    try {
        std::size_t i;
        auto v = std::stoi(str, &i, 10);
        if (i == str.length()) {
            return v;
        }
    } catch (...) {
        // not an integer
    }
    return shard::nullopt;
}

int main(int /* argc */, char* /* argv */[]) {
#if __cplusplus >= 201703L
    static_assert(sizeof(std::optional<char>) == sizeof(shard::optional<char>));
    static_assert(sizeof(std::optional<int>) == sizeof(shard::optional<int>));
    static_assert(sizeof(std::optional<void*>) == sizeof(shard::optional<void*>));
#endif

    if (auto i = to_int("42")) {
        std::cout << (*i / 2) << '\n';
    }

    return 0;
}
