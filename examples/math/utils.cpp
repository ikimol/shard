// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/math/utils.hpp>

using namespace shard;

int main(int /* argc */, char* /* argv */[]) {
    static_assert(math::clamp(15, 0, 10) == 10, "clamp");
    static_assert(math::clamp(-5, 0, 10) == 0, "clamp");
    static_assert(math::clamp(5, 0, 10) == 5, "clamp");

    static_assert(math::sq(-5) == 25, "sq");
    static_assert(math::sq(5) == 25, "sq");

    static_assert(math::pct(10, 0, 10) == 1.f, "pct");
    static_assert(math::pct(0, 0, 10) == 0.f, "pct");
    static_assert(math::pct(2, 0, 10) == 0.2f, "pct");

    return 0;
}
