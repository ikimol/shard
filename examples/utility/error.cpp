// Copyright (c) 2022 Miklos Molnar. All rights reserved.

#include <shard/utility/error.hpp>

#include <iostream>

class my_error : public shard::error {
public:
    explicit my_error(const char* message) : shard::error(42, message) {}

    explicit my_error(const std::string& message) : shard::error(42, message) {}
};

int main(int /* argc */, char* /* argv */[]) {
    try {
        throw my_error("foobar");
    } catch (my_error& e) {
        std::cerr << '[' << e.code() << "]: " << e.what() << '\n';
    }
    return 0;
}