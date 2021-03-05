// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_TEST_NO_DEFAULT_HPP
#define SHARD_TEST_NO_DEFAULT_HPP

namespace test {

struct no_default {
    no_default(int a, int b) : a(a), b(b) {}

    int a;
    int b;
};

} // namespace test

#endif // SHARD_TEST_NO_DEFAULT_HPP
