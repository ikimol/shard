// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/property.hpp>

#include <iostream>
#include <string>

class widget {
public:
    widget() = default;

private:
    void update() const { std::cout << "updated: " << *name << '\n'; }

public:
    shard::observed_property<int> id {[this] { update(); }, 0};

    const shard::property<std::string> name {[this] {
        return "w." + std::to_string(*id);
    }};
};

int main(int /* argc */, char* /* argv */[]) {
    widget w;
    w.id = 42;
    w.id = 21;

    return 0;
}
