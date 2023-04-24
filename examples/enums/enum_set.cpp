// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/enums/enum_set.hpp>

#include <iostream>

enum class target : char { player, enemy, bullet };

int main(int /* argc */, char* /* argv */[]) {
    std::cout << "sizeof(target): " << sizeof(target) << '\n';

    shard::enum_set<target, 3> targets;
    std::cout << "count: " << targets.count() << '\n';

    targets.set_all();
    std::cout << "count: " << targets.count() << '\n';

    targets.clear();
    targets.set(target::player);
    std::cout << "player: " << targets.test(target::player) << '\n';
    std::cout << "enemy: " << targets.test(target::enemy) << '\n';
    std::cout << "bullet: " << targets.test(target::bullet) << '\n';

    targets[target::enemy] = false;
    targets[target::bullet] = true;

    for (auto t : targets) {
        std::cout << (int)(t) << '\n';
    }

    return 0;
}