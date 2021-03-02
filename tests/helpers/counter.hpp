// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_TEST_COUNTER_HPP
#define SHARD_TEST_COUNTER_HPP

#include <cstddef>

namespace test {

struct counter {
    counter() {
        ++default_constructor;
        ++instaces;
    }

    counter(const counter&) {
        ++copy_constructor;
        ++instaces;
    }

    counter(counter&&) noexcept {
        ++move_constructor;
        ++instaces;
    }

    ~counter() {
        ++destructor;
        --instaces;
    }

    counter& operator=(const counter&) {
        ++copy_assignment_op;
        return *this;
    }

    counter& operator=(counter&&) noexcept {
        ++move_assignment_op;
        return *this;
    }

    static void reset() {
        instaces = 0;
        default_constructor = 0;
        copy_constructor = 0;
        move_constructor = 0;
        destructor = 0;
        copy_assignment_op = 0;
        move_assignment_op = 0;
    }

    static std::size_t instaces; // the number of object instances

    static std::size_t default_constructor;
    static std::size_t copy_constructor;
    static std::size_t move_constructor;
    static std::size_t destructor;
    static std::size_t copy_assignment_op;
    static std::size_t move_assignment_op;
};

std::size_t counter::instaces = 0;
std::size_t counter::default_constructor = 0;
std::size_t counter::copy_constructor = 0;
std::size_t counter::move_constructor = 0;
std::size_t counter::destructor = 0;
std::size_t counter::copy_assignment_op = 0;
std::size_t counter::move_assignment_op = 0;

} // namespace test

#endif // SHARD_TEST_COUNTER_HPP
