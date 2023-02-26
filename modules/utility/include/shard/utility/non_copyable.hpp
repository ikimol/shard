// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

namespace shard {
namespace utility {

class non_copyable {
public:
    non_copyable(const non_copyable&) = delete;
    non_copyable& operator=(const non_copyable&) = delete;

protected:
    non_copyable() = default;
    ~non_copyable() = default;
};

} // namespace utility

// bring symbols into parent namespace

using utility::non_copyable;

} // namespace shard
