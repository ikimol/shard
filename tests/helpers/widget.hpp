// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_TEST_WIDGET_HPP
#define SHARD_TEST_WIDGET_HPP

namespace test {

struct widget {
    widget() = default;

    widget(int a, int b) : a(a), b(b) {}

    explicit widget(int n) : a(n), b(n) {}

    bool test(int x, int y) const { return x == a && y == b; }

    int a = 0;
    int b = 0;
};

} // namespace test

#endif // SHARD_TEST_WIDGET_HPP
