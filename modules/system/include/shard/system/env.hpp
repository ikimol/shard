// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_SYSTEM_ENV_HPP
#define SHARD_SYSTEM_ENV_HPP

#include <map>
#include <optional>
#include <string>

namespace shard {
namespace system::env {

/// Check if an environment variable is set
///
/// \note If the value of the variable will be needed then it is slightly more
/// efficient to call os::env::get() which returns a nullopt if the variable is
/// not set.
bool has(const std::string& key);

/// Get the value of an environment variable or nullopt if it is not set
std::optional<std::string> get(const std::string& key);

/// Set the value of an environment variable or override it if it is already set
bool set(const std::string& key, const std::string& value, std::string* old_value = nullptr);

/// Unset the environment variable
bool unset(const std::string& key, std::string* old_value = nullptr);

/// Get the environment variables as key-value pairs
const std::map<std::string, std::string>& vars();

} // namespace system::env

// bring symbols into main namespace

namespace env {

using system::env::get;
using system::env::has;
using system::env::set;
using system::env::unset;
using system::env::vars;

} // namespace env

} // namespace shard

#include "shard/system/env.inl"

#endif // SHARD_SYSTEM_ENV_HPP
