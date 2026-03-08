// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#include <shard/string/search.hpp>

#include <iostream>

int main(int /* argc */, char* /* argv */[]) {
    std::string s = "aabaacaadaabaabaaaba";
    std::string pattern = "aaba";

    std::vector<std::size_t> indices;
    shard::search(s, pattern, std::back_inserter(indices));
    for (auto i : indices) {
        std::cout << i << " ";
    }

    return 0;
}
