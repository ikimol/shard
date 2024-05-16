// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

namespace shard {
namespace concurrency::this_thread {

/// Set the name of the current thread
void set_name(const char* name);

} // namespace concurrency::this_thread

// bring symbols into parent namespace

namespace this_thread = concurrency::this_thread;

} // namespace shard
