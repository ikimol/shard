// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_UTILITY_NON_COPYABLE_HPP
#define SHARD_UTILITY_NON_COPYABLE_HPP

namespace shard {
namespace core {

class non_copyable {
public:
    non_copyable(const non_copyable&) = delete;
    non_copyable& operator=(const non_copyable&) = delete;

protected:
    non_copyable() = default;
    ~non_copyable() = default;
};

} // namespace core

// bring symbols into parent namespace

using core::non_copyable;

} // namespace shard

#endif // SHARD_UTILITY_NON_COPYABLE_HPP
