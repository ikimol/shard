// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_TEST_ADDRESSER_HPP
#define SHARD_TEST_ADDRESSER_HPP

namespace test {

template <typename T>
struct addresser {
    explicit addresser(const T* p)
    : ptr(p) {}

    const T** operator&() { /* NOLINT */
        return &ptr;
    }

    const T* ptr;
};

} // namespace test

#endif // SHARD_TEST_ADDRESSER_HPP
