// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <algorithm>
#include <optional>
#include <set>
#include <vector>

namespace shard {
namespace algorithm {

/// Insert a value into the container, keeping its order
///
/// \note Supported containers: deque, list, map, set, string, vector
template <typename Container, typename T>
auto insert_ordered(Container& c, const T& val) -> decltype(std::begin(c)) {
    auto it = std::lower_bound(std::begin(c), std::end(c), val);
    return c.insert(it, val);
}

/// Remove an iterator from the container, breaking its order
///
/// \note Supported containers: deque, list, string, vector
template <typename Container, typename Iterator>
void remove_unordered(Container& c, Iterator it) {
    *it = std::move(c.back());
    c.pop_back();
}

/// Remove all occurrences of the value from the container
///
/// \note Supports: deque, list, map, unordered_map, string, set, unordered_set,
///                 vector
template <typename Container, typename T>
void erase(Container& c, const T& val) {
    c.erase(std::remove(std::begin(c), std::end(c), val), c.end());
}

/// Remove all occurrences of the values from the container that match the
/// predicate
///
/// \note Supports: deque, list, map, unordered_map, string, set, unordered_set,
///                 vector
template <typename Container, typename UnaryPred>
void erase_if(Container& c, UnaryPred pred) {
    c.erase(std::remove_if(std::begin(c), std::end(c), pred), c.end());
}

/// Get the value if the key exists
///
/// \note Supports: map, unordered_map
template <typename Container, typename T>
auto get_value(Container& c, const T& key) -> std::optional<typename Container::mapped_type> {
    auto it = c.find(key);
    if (it != c.end()) {
        return it->second;
    }
    return std::nullopt;
}

/// Check if the key exists
///
/// \note Supports: map, unordered_map, set, unordered_set
template <typename Container, typename T>
bool has_key(Container&& c, const T& key) {
    return c.find(key) != c.end();
}

/// Get all keys of the map
///
/// \note Supports: map, unordered_map
template <typename Map, typename OutputIt>
void keys_of(const Map& m, OutputIt it) {
    for ([[maybe_unused]] auto& [key, _] : m) {
        *it = key;
    }
}

/// Copy all keys of the map into a set
///
/// \note Supports: map, unordered_map
template <typename Map>
std::set<typename Map::key_type> keys_of(const Map& m) {
    std::set<typename Map::key_type> s;
    keys_of(m, std::inserter(s, s.end()));
    return s;
}

/// Get all values of the map
///
/// \note Supports: map, unordered_map
template <typename Map, typename OutputIt>
void values_of(const Map& m, OutputIt it) {
    for ([[maybe_unused]] auto& [_, value] : m) {
        *it = value;
    }
}

/// Copy all values of the map into a vector
///
/// \note Supports: map, unordered_map
template <typename Map>
std::vector<typename Map::mapped_type> values_of(const Map& m) {
    std::vector<typename Map::mapped_type> v;
    values_of(m, std::back_inserter(v));
    return v;
}

} // namespace algorithm

// bring symbols into parent namespace

using algorithm::erase;
using algorithm::erase_if;
using algorithm::get_value;
using algorithm::has_key;
using algorithm::insert_ordered;
using algorithm::keys_of;
using algorithm::remove_unordered;
using algorithm::values_of;

} // namespace shard
