// Copyright (c) 2022 Miklos Molnar. All rights reserved.

#include <shard/utility/timer.hpp>

#include <iostream>
#include <thread>

int main(int /* argc */, char* /* argv */[]) {
    shard::timer::duration d;
    {
        shard::timer timer(d);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(d).count() << "ms\n";
    return 0;
}
