// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/string/to_number.hpp>

#include <iostream>

#define convert(fn, str)                                                                                               \
    if (auto x = shard::fn(str)) {                                                                                     \
        std::cout << *x << '\n';                                                                                       \
    }

int main(int /* argc */, char* /* argv */[]) {
    std::cout << "to_int\n";

    convert(to_int, "41");
    convert(to_int, "-41");
    convert(to_int, "41.2");
    convert(to_int, "foo");

    std::cout << "to_unsigned\n";

    convert(to_unsigned, "41");
    convert(to_unsigned, "-41");
    convert(to_unsigned, "41.2");
    convert(to_unsigned, "foo");

    std::cout << "to_float\n";

    convert(to_float, "41");
    convert(to_float, "-41");
    convert(to_float, "41.2");
    convert(to_float, "foo");

    std::cout << "to_double\n";

    convert(to_double, "41");
    convert(to_double, "-41");
    convert(to_double, "41.2");
    convert(to_double, "foo");

    return 0;
}
