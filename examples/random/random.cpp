// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/random.hpp>

#include <ctime>
#include <iostream>
#include <map>

int main(int /* argc */, char* /* argv */[]) {
    shard::random<int> r_int(0, 10);
    std::cout << r_int.next() << '\n';

    shard::random<float> r_float(0.5f, 1.5f);
    std::cout << r_float.next() << '\n';

    // query the seeds of the generators
    std::cout << "r_int.seed = " << r_int.seed() << '\n';
    std::cout << "r_float.seed = " << r_float.seed() << '\n';

    // set the seed of the generator
    r_float.reseed();
    std::cout << r_float.next() << '\n';

    // generates a random boolean that will be true 25% of the time
    shard::random<bool> r_bool(0.25);
    r_bool.reseed();
    std::map<bool, int> histogram;
    for (int n = 0; n < 10000; ++n) {
        ++histogram[r_bool.next()];
    }
    for (auto p : histogram) {
        std::cout << p.first << ' ' << std::string(p.second / 500, '*') << '\n';
    }

    // generates a random number using a faster method than the above
    std::cout << shard::fast_random(0, 100) << '\n';

    // set the seed used by 'fast_random'
    shard::fast_random_seed(std::time(nullptr));
    std::cout << shard::fast_random(0, 10) << '\n';

    return 0;
}
