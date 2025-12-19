// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/io/file.hpp>

#include <filesystem>
#include <iostream>

using namespace std::string_view_literals;

int main(int /* argc */, char* /* argv */[]) {
    auto path = std::filesystem::current_path() / "test.txt";

    std::filesystem::remove(path);

    {
        shard::file f(path.c_str(), shard::io::open_mode::write);
        auto str = "foobar"sv;
        f.write(str.data(), str.size());
        int i = 42;
        f.write(&i, 1);
    }

    {
        shard::file f(path.c_str());
        char buffer[6] = {};
        f.read(buffer, 1, 6);
        int i;
        f.read(&i, 1);
        std::cout << buffer << ':' << i << '\n';
    }

    return 0;
}
