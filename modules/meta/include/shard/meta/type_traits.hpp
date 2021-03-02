// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_META_TYPE_TRAITS_HPP
#define SHARD_META_TYPE_TRAITS_HPP

#include <type_traits>

namespace shard {
namespace meta {
namespace detail {

// clang-format off
template <typename T> struct is_integer_impl : std::false_type {};

// signed
template <> struct is_integer_impl<char> : std::true_type {};
template <> struct is_integer_impl<short> : std::true_type {};
template <> struct is_integer_impl<int> : std::true_type {};
template <> struct is_integer_impl<long> : std::true_type {};
template <> struct is_integer_impl<long long> : std::true_type {};

// unsigned
template <> struct is_integer_impl<unsigned char> : std::true_type {};
template <> struct is_integer_impl<unsigned short> : std::true_type {};
template <> struct is_integer_impl<unsigned int> : std::true_type {};
template <> struct is_integer_impl<unsigned long> : std::true_type {};
template <> struct is_integer_impl<unsigned long long> : std::true_type {};
// clang-format on

struct stream_test {
    template <typename S, typename T>
    static auto test(int) -> decltype(std::declval<S&>() << std::declval<T>(), std::true_type()) {}

    template <typename, typename>
    static auto test(...) -> std::false_type {}
};

} // namespace detail

// is_<type> structs

template <typename T>
struct is_integer : detail::is_integer_impl<std::remove_cv_t<T>> {};

template <typename T>
struct is_bool : std::is_same<std::remove_cv_t<T>, bool> {};

template <typename T>
struct is_numeric : std::integral_constant<bool, is_integer<T>::value || std::is_floating_point<T>::value> {};

template <typename S, typename T>
struct is_streamable : std::integral_constant<bool, decltype(detail::stream_test::test<S, T>(0))::value> {};

// are_same

template <typename T, typename...>
struct are_same : std::true_type {};

template <typename T, typename U, typename... Args>
struct are_same<T, U, Args...> :
std::integral_constant<bool, std::is_same<T, U>::value && are_same<T, Args...>::value> {};

// operators

template <typename T>
using not_type = std::integral_constant<bool, !T::value>;

template <typename Condition, typename Then, typename Else>
using if_type = std::conditional_t<Condition::value, Then, Else>;

template <typename... Args>
struct and_type : std::integral_constant<bool, true> {};

template <typename T, typename... Args>
struct and_type<T, Args...> : if_type<T, and_type<Args...>, std::integral_constant<bool, false>> {};

template <typename... Args>
struct or_type : std::integral_constant<bool, false> {};

template <typename T, typename... Args>
struct or_type<T, Args...> : if_type<T, std::integral_constant<bool, true>, or_type<Args...>> {};

template <typename... Args>
using enable_if_all_t = std::enable_if_t<and_type<Args...>::value, int>;

template <typename... Args>
using enable_if_any_t = std::enable_if_t<or_type<Args...>::value, int>;

// disable_if

// clang-format off
template <bool, typename T = void> struct disable_if {};
template <typename T> struct disable_if<false, T> { using type = T; };
// clang-format on

template <bool Bool, typename T = void>
using disable_if_t = typename disable_if<Bool, T>::type;

// function result

// clang-format off
template <typename> struct result_of;
template <class R, class... Args> struct result_of<R(Args...)> { using type = R; };
template <typename R, typename... Args> struct result_of<R(*)(Args...)> { using type = R; };
// clang-format on

template <typename T>
using result_of_t = typename result_of<T>::type;

// void_t

// clang-format off
template <typename...> struct make_void { using type = void; };
template <typename... Ts> using void_t = typename make_void<Ts...>::type;
// clang-format on

} // namespace meta

using meta::is_bool;
using meta::is_integer;
using meta::is_numeric;
using meta::is_streamable;

using meta::are_same;

using meta::and_type;
using meta::if_type;
using meta::not_type;
using meta::or_type;

using meta::enable_if_all_t;
using meta::enable_if_any_t;

using meta::disable_if;
using meta::disable_if_t;

using meta::result_of_t;
using meta::void_t;

} // namespace shard

#endif // SHARD_META_TYPE_TRAITS_HPP
