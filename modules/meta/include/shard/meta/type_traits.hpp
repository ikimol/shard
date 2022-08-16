// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_META_TYPE_TRAITS_HPP
#define SHARD_META_TYPE_TRAITS_HPP

#include <tuple>
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

template <typename S, typename T>
class is_streamable_impl {
private:
    template <typename S1, typename T1>
    static auto test(int) -> decltype(std::declval<S1&>() << std::declval<T1>(), std::true_type());

    template <typename, typename>
    static auto test(...) -> std::false_type;

public:
    static const bool value = decltype(test<S, T>(0))::value;
};

template <typename T>
class has_begin_end_impl {
private:
    template <typename U = std::decay_t<T>, typename B = decltype(std::declval<U&>().begin()),
    typename E = decltype(std::declval<U&>().end())>
    static std::true_type test(int);

    template <typename...>
    static auto test(...) -> std::false_type;

public:
    static const bool value = decltype(test<T>(0))::value;
};

template <typename T>
class has_key_value_pair_impl {
private:
    template <typename U = std::decay_t<T>, typename V = typename U::value_type,
    typename F = decltype(std::declval<V&>().first), typename S = decltype(std::declval<V&>().second)>
    static std::true_type test(int);

    template <typename...>
    static auto test(...) -> std::false_type;

public:
    static const bool value = decltype(test<T>(0))::value;
};

} // namespace detail

// is_<type> structs

template <typename T>
struct is_integer : detail::is_integer_impl<std::remove_cv_t<T>> {};

template <typename T>
struct is_bool : std::is_same<std::remove_cv_t<T>, bool> {};

template <typename T>
struct is_numeric : std::bool_constant<is_integer<T>::value || std::is_floating_point<T>::value> {};

template <typename S, typename T>
struct is_streamable : std::bool_constant<detail::is_streamable_impl<S, T>::value> {};

template <typename T>
struct has_begin_end : std::bool_constant<detail::has_begin_end_impl<T>::value> {};

template <typename T>
struct has_key_value_pair : std::bool_constant<detail::has_key_value_pair_impl<T>::value> {};

// is_empty

template <typename... Args>
struct is_empty : std::bool_constant<sizeof...(Args) == 0> {};

// are_same

template <typename T, typename...>
struct are_same : std::true_type {};

template <typename T, typename U, typename... Args>
struct are_same<T, U, Args...> : std::bool_constant<std::is_same<T, U>::value && are_same<T, Args...>::value> {};

// operators

template <typename T>
using not_type = std::bool_constant<!T::value>;

template <typename Condition, typename Then, typename Else>
using if_type = std::conditional_t<Condition::value, Then, Else>;

template <typename... Args>
struct and_type : std::bool_constant<true> {};

template <typename T, typename... Args>
struct and_type<T, Args...> : if_type<T, and_type<Args...>, std::bool_constant<false>> {};

template <typename... Args>
struct or_type : std::bool_constant<false> {};

template <typename T, typename... Args>
struct or_type<T, Args...> : if_type<T, std::bool_constant<true>, or_type<Args...>> {};

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

// function traits

// clang-format off
template <typename> struct result_of;
template <class R, class... Args> struct result_of<R(Args...)> { using type = R; };
template <typename R, typename... Args> struct result_of<R(*)(Args...)> { using type = R; };
// clang-format on

template <typename T>
struct functor_traits : public functor_traits<decltype(&T::operator())> {};

template <typename T, typename R, typename... Args>
struct functor_traits<R (T::*)(Args...) const> {
    static constexpr auto arity = sizeof...(Args);

    typedef R result_type;
    using args_type = std::tuple<Args...>;

    template <std::size_t N>
    using arg_type = typename std::tuple_element<N, args_type>::type;
};

template <typename T>
using result_of_t = typename result_of<T>::type;

} // namespace meta

using meta::is_bool;
using meta::is_integer;
using meta::is_numeric;
using meta::is_streamable;

using meta::has_begin_end;
using meta::has_key_value_pair;

using meta::are_same;
using meta::is_empty;

using meta::and_type;
using meta::if_type;
using meta::not_type;
using meta::or_type;

using meta::enable_if_all_t;
using meta::enable_if_any_t;

using meta::disable_if;
using meta::disable_if_t;

using meta::functor_traits;
using meta::result_of_t;

} // namespace shard

#endif // SHARD_META_TYPE_TRAITS_HPP
