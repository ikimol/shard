// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include <functional>

namespace shard {

/// Bind a class method as a function
template <typename T, typename... Args>
std::function<void(Args...)> bind(T* object, void (T::*method)(Args...)) {
    return [object, method](Args&&... args) { return (object->*method)(std::forward<Args>(args)...); };
}

} // namespace shard
