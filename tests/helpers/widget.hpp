// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_TEST_WIDGET_HPP
#define SHARD_TEST_WIDGET_HPP

namespace test {

struct widget {
    widget() = default;

    explicit widget(int a, int b) : a(a), b(b) {}

    int a = 0;
    int b = 0;
};

} // namespace test

#endif // SHARD_TEST_WIDGET_HPP
