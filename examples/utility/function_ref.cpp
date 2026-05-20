// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#include <shard/utility/function_ref.hpp>

#include <iostream>

static void apply(shard::function_ref<int(int)> f) {
    std::cout << "result = " << f(21) << '\n';
}

int main(int /* argc */, char* /* argv */[]) {
    apply([](int i) { return i * 2; });
    return 0;
}
