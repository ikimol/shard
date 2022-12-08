// Copyright (c) 2022 Miklos Molnar. All rights reserved.

#include <shard/containers/static_vector.hpp>

#include <iostream>

int main(int /* argc */, char* /* argv */[]) {
    shard::static_vector<int, 10> v;
    std::cout << sizeof(v) << " bytes\n";

    v.emplace_back(10);
    std::cout << v.first() << '\n';

    v.push_back(20);
    std::cout << v.last() << '\n';

    auto value = 30;
    v.push_back(value);

    for (auto i : v) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    std::cout << "size: " << v.size() << '\n';

    v.pop_back();
    std::cout << "size: " << v.size() << '\n';

    std::cout << *(v.data()) << '\n';

    v.clear();
    std::cout << "size: " << v.size() << '\n';

    return 0;
}