// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include <shard/algorithm/functional.hpp>
#include <shard/algorithm/functional/vector.hpp>

#include <iostream>
#include <string>
#include <vector>

static std::optional<int> parse_int(const std::string& s) {
    try {
        std::size_t index;
        if (auto i = std::stoi(s, &index, 10); index == s.size()) {
            return i;
        }
    } catch (...) {}
    return std::nullopt;
}

int main(int /* argc */, char* /* argv */[]) {
    std::vector chars = {'0', '1', 'a', '2', '-', '3', '@', ' ', '4'};
    auto strings = shard::functional::map(chars, [](char c) { return std::string(1, c); });
    auto numbers = shard::functional::flat_map(strings, parse_int);
    auto even_numbers = shard::functional::filter(numbers, [](auto i) { return i % 2 == 0; });
    std::vector names = {"foo", "bar", "baz"};
    auto pairs = shard::functional::zip(names, even_numbers);

    for (const auto& [s, i] : pairs) {
        std::cout << s << ": " << i << '\n';
    }

    auto sum = shard::functional::reduce(pairs, 0, [](auto&& result, const auto& p) { result += p.second; });
    std::cout << sum << '\n';

    return 0;
}
