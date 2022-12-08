// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include "helpers/counter.hpp"

namespace test {

std::size_t counter::instances = 0;
std::size_t counter::default_constructor = 0;
std::size_t counter::copy_constructor = 0;
std::size_t counter::move_constructor = 0;
std::size_t counter::destructor = 0;
std::size_t counter::copy_assignment_op = 0;
std::size_t counter::move_assignment_op = 0;

} // namespace test
