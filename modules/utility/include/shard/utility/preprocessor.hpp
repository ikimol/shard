// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_UTILITY_PREPROCESSOR_HPP
#define SHARD_UTILITY_PREPROCESSOR_HPP

/// Extract the head of the argument list
#define SHARD_VA_HEAD(x, ...) x

/// Extract the tail of the argument list
#define SHARD_VA_TAIL(x, ...) __VA_ARGS__

/// Denote a variable as unused to avoid compiler warnings
#define SHARD_UNUSED(x) ((void)(x))

/// Concatenate the two arguments
#define __SHARD_CONCAT(a, b) a##b
#define SHARD_CONCAT(a, b) __SHARD_CONCAT(a, b)

/// Create a locally (current TU) unique identifier
#define SHARD_UNIQUE_ID(x) SHARD_CONCAT(x, __COUNTER__)

/// Convert the argument to a string verbatim
#define SHARD_STRINGIZE(x) #x

/// Forwards the argument
#define SHARD_FWD(x) std::forward<decltype(x)>(x)

#endif // SHARD_UTILITY_PREPROCESSOR_HPP
