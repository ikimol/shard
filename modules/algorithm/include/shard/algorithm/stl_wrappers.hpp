// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_ALGORITHM_STL_WRAPPERS_HPP
#define SHARD_ALGORITHM_STL_WRAPPERS_HPP

#include <algorithm>

namespace shard {
namespace algorithm {

/// Convenience wrapper for 'std::distance'
template <typename Range>
auto size(Range&& r) -> decltype(std::distance(std::begin(r), std::end(r))) {
    return std::distance(std::begin(r), std::end(r));
}

/// Convenience wrapper for 'std::for_each'
template <typename Range, typename UnaryPred>
UnaryPred for_each(Range&& r, UnaryPred pred) {
    return std::for_each(std::begin(r), std::end(r), pred);
}

/// Convenience wrapper for 'std::all_of'
template <typename Range, typename UnaryPred>
bool all_of(Range&& r, UnaryPred pred) {
    return std::all_of(std::begin(r), std::end(r), pred);
}

/// Convenience wrapper for 'std::any_of'
template <typename Range, typename UnaryPred>
bool any_of(Range&& r, UnaryPred pred) {
    return std::any_of(std::begin(r), std::end(r), pred);
}

/// Convenience wrapper for 'std::none_of'
template <typename Range, typename UnaryPred>
bool none_of(Range&& r, UnaryPred pred) {
    return std::none_of(std::begin(r), std::end(r), pred);
}

/// Convenience wrapper for 'std::find'
template <typename Range, typename T>
auto find(Range&& r, const T& val) -> decltype(std::begin(r)) {
    return std::find(std::begin(r), std::end(r), val);
}

/// Convenience wrapper for 'std::find_if'
template <typename Range, typename UnaryPred>
auto find_if(Range&& r, UnaryPred pred) -> decltype(std::begin(r)) {
    return std::find_if(std::begin(r), std::end(r), pred);
}

/// Convenience wrapper for 'std::find_if_not'
template <typename Range, typename UnaryPred>
auto find_if_not(Range&& r, UnaryPred pred) -> decltype(std::begin(r)) {
    return std::find_if_not(std::begin(r), std::end(r), pred);
}

/// Convenience wrapper for 'std::remove_if'
template <typename Range, typename UnaryPred>
auto remove_if(Range&& r, UnaryPred pred) -> decltype(std::begin(r)) {
    return std::remove_if(std::begin(r), std::end(r), pred);
}

/// Convenience wrapper for 'std::copy'
template <typename Range, typename OutputIt>
OutputIt copy(Range&& r, OutputIt it) {
    return std::copy(std::begin(r), std::end(r), it);
}

/// Convenience wrapper for 'std::copy_if'
template <typename Range, typename OutputIt, typename UnaryPred>
OutputIt copy_if(Range&& r, OutputIt it, UnaryPred pred) {
    return std::copy_if(std::begin(r), std::end(r), it, pred);
}

/// Convenience wrapper for 'std::count'
template <typename Range, typename T>
auto count(Range&& r, const T& val) -> typename std::iterator_traits<decltype(std::begin(r))>::difference_type {
    return std::count(std::begin(r), std::end(r), val);
}

/// Convenience wrapper for 'std::count_if'
template <typename Range, typename UnaryPred>
auto count_if(Range&& r, UnaryPred pred) -> typename std::iterator_traits<decltype(std::begin(r))>::difference_type {
    return std::count_if(std::begin(r), std::end(r), pred);
}

/// Convenience wrapper for 'std::transform'
template <typename Range, typename OutputIt, typename UnaryPredicate>
OutputIt transform(Range&& r, OutputIt it, UnaryPredicate pred) {
    return std::transform(std::begin(r), std::end(r), it, pred);
}

/// Check if the range contains the value
template <typename Range, typename T>
bool contains(Range&& r, const T& val) {
    return find(r, val) != std::end(r);
}

/// Get the index of the value in the range
template <typename Range, typename T>
auto index_of(Range&& r, const T& val) -> typename std::iterator_traits<decltype(std::begin(r))>::difference_type {
    return std::distance(std::begin(r), find(r, val));
}

} // namespace algorithm

// bring symbols into parent namespace

using algorithm::all_of;
using algorithm::any_of;
using algorithm::contains;
using algorithm::copy;
using algorithm::copy_if;
using algorithm::count;
using algorithm::count_if;
using algorithm::find;
using algorithm::find_if;
using algorithm::find_if_not;
using algorithm::for_each;
using algorithm::index_of;
using algorithm::none_of;
using algorithm::remove_if;
using algorithm::size;
using algorithm::transform;

} // namespace shard

#endif // SHARD_ALGORITHM_STL_WRAPPERS_HPP
