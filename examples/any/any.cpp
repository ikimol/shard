// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/any.hpp>

#include <iostream>
#include <string>

int main(int /* argc */, char* /* argv */[]) {
    shard::any a;
    std::cout << std::boolalpha << a.has_value() << '\n';

    std::cout << "sizeof(shard::any) = " << sizeof(shard::any) << '\n';

    a = 3.14;
    std::cout << a.type().name() << '\n';
    a.reset();
    std::cout << a.has_value() << '\n';

    {
        using namespace std::string_literals;
        a = "hello"s;
        std::cout << shard::any_cast<const std::string&>(a) << '\n';
    }

    try {
        a = 42;
        shard::any_cast<float>(a);
    } catch (std::exception& e) {
        // print error
        std::cerr << e.what() << '\n';
    }

    return 0;
}
