// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <memory>

namespace shard {
namespace stlxt {

template <typename T, typename Allocator = std::allocator<T>>
class default_init_allocator : public Allocator {
    using traits = std::allocator_traits<Allocator>;

public:
    template <typename U>
    struct rebind {
        using other = default_init_allocator<U, typename traits::template rebind_alloc<U>>;
    };

    using Allocator::Allocator;

    template <typename U>
    void construct(U* ptr) noexcept(std::is_nothrow_default_constructible_v<U>) {
        ::new (static_cast<void*>(ptr)) U;
    }

    template <typename U, typename... Args>
    void construct(U* ptr, Args&&... args) {
        traits::construct(static_cast<Allocator&>(*this), ptr, std::forward<Args>(args)...);
    }
};

} // namespace stlxt

// bring symbols into parent namespace

using stlxt::default_init_allocator;

} // namespace shard
