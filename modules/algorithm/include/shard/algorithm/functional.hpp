// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/meta/type_traits.hpp>

namespace shard {
namespace algorithm::functional {
namespace detail {

template <typename Container>
struct inserter;

template <typename Container, typename... Args>
void insert(Container& c, Args&&... args) {
    inserter<Container>::insert(c, std::forward<Args>(args)...);
}

template <typename Container>
struct reserver {
    static void reserve(Container&, std::size_t) {}
};

template <typename Container>
void reserve(Container& c, std::size_t new_cap) {
    reserver<Container>::reserve(c, new_cap);
}

template <typename T>
using cref_arg_t = std::add_const_t<std::add_lvalue_reference_t<unqualified_t<T>>>;

template <typename Predicate, typename T>
inline constexpr bool is_map_predicate_v = !std::is_void_v<std::invoke_result_t<Predicate, cref_arg_t<T>>>;

template <typename UnaryPredicate, typename T>
using map_predicate_result_t = std::invoke_result_t<UnaryPredicate, cref_arg_t<T>>;

template <typename Predicate, typename T>
inline constexpr bool is_flat_map_predicate_v = is_optional_v<std::invoke_result_t<Predicate, cref_arg_t<T>>>;

template <typename UnaryPredicate, typename T>
using flat_map_predicate_result_t = typename std::invoke_result_t<UnaryPredicate, cref_arg_t<T>>::value_type;

template <typename Predicate, typename T>
inline constexpr bool is_filter_predicate_v =
    std::is_nothrow_constructible_v<T, std::invoke_result_t<Predicate, cref_arg_t<T>>>;

template <typename Predicate, typename T, typename U>
inline constexpr bool is_reduce_predicate_v =
    std::is_void_v<std::invoke_result_t<Predicate, std::add_lvalue_reference_t<unqualified_t<U>>, cref_arg_t<T>>>;

} // namespace detail

/// Transform a container of values using the predicate
///
/// \note The type of the result container is allowed to differ from the
/// source container.
template <template <typename...> typename Container,
          template <typename...> typename Result = Container,
          typename T,
          typename UnaryPredicate,
          typename = std::enable_if_t<detail::is_map_predicate_v<UnaryPredicate, T>>>
[[nodiscard]] auto map(const Container<T>& c, UnaryPredicate&& p) {
    Result<detail::map_predicate_result_t<UnaryPredicate, T>> result;
    detail::reserve(result, c.size());
    for (const auto& v : c) {
        detail::insert(result, p(v));
    }
    return result;
}

/// Transform a container of values using the predicate, ignoring values for
/// which \c std::nullopt is returned.
///
/// \note The type of the result container is allowed to differ from the
/// source container.
template <template <typename...> typename Container,
          template <typename...> typename Result = Container,
          typename T,
          typename UnaryPredicate,
          typename = std::enable_if_t<detail::is_flat_map_predicate_v<UnaryPredicate, T>>>
[[nodiscard]] auto flat_map(const Container<T>& c, UnaryPredicate&& p) {
    Result<detail::flat_map_predicate_result_t<UnaryPredicate, T>> result;
    for (const auto& v : c) {
        if (auto opt = p(v)) {
            detail::insert(result, *opt);
        }
    }
    return result;
}

/// Filter a container with the given predicate
template <template <typename...> typename Container,
          typename T,
          typename UnaryPredicate,
          typename = std::enable_if_t<detail::is_filter_predicate_v<UnaryPredicate, T>>>
[[nodiscard]] Container<T> filter(const Container<T>& c, UnaryPredicate&& p) {
    Container<T> result;
    for (const auto& v : c) {
        if (p(v)) {
            detail::insert(result, v);
        }
    }
    return result;
}

/// Create a container of pairs from two given containers
template <template <typename...> typename Container, typename T, typename U>
[[nodiscard]] Container<std::pair<T, U>> zip(const Container<T>& lhs, const Container<U>& rhs) {
    Container<std::pair<T, U>> result;
    detail::reserve(result, std::min(lhs.size(), rhs.size()));
    auto lhs_it = lhs.begin();
    auto rhs_it = rhs.begin();
    while (lhs_it != lhs.end() && rhs_it != rhs.end()) {
        detail::insert(result, std::make_pair(*lhs_it++, *rhs_it++));
    }
    return result;
}

/// Combine the elements of the given cointainer using the given predicate
template <template <typename...> typename Container,
          typename T,
          typename U,
          typename BinaryPredicate,
          typename = std::enable_if_t<detail::is_reduce_predicate_v<BinaryPredicate, T, U>>>
[[nodiscard]] U reduce(const Container<T>& c, U result, BinaryPredicate&& p) {
    for (const auto& v : c) {
        p(result, v);
    }
    return result;
}

} // namespace algorithm::functional

// bring symbols into parent namespace

namespace functional = algorithm::functional;

} // namespace shard
