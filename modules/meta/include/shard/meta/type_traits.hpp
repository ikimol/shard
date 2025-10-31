// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <functional>
#include <optional>
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

template <typename T, template <typename...> class Template>
struct is_specialization_of_impl : std::false_type {};

template <typename... T, template <typename...> class Template>
struct is_specialization_of_impl<Template<T...>, Template> : std::true_type {};

template <typename T>
class has_begin_end_impl {
private:
    template <typename U = std::decay_t<T>,
              typename B = decltype(std::declval<U&>().begin()),
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
    template <typename U = std::decay_t<T>,
              typename V = typename U::value_type,
              typename F = decltype(std::declval<V&>().first),
              typename S = decltype(std::declval<V&>().second)>
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
inline constexpr bool is_integer_v = is_integer<T>::value;

template <typename T>
struct is_bool : std::is_same<std::remove_cv_t<T>, bool> {};

template <typename T>
inline constexpr bool is_bool_v = is_bool<T>::value;

template <typename T>
struct is_numeric : std::bool_constant<is_integer_v<T> || std::is_floating_point_v<T>> {};

template <typename T>
inline constexpr bool is_numeric_v = is_numeric<T>::value;

template <typename S, typename T>
struct is_streamable : std::bool_constant<detail::is_streamable_impl<S, T>::value> {};

template <typename S, typename T>
inline constexpr bool is_streamable_v = is_streamable<S, T>::value;

template <typename T, template <typename...> class Template>
using is_specialization_of = detail::is_specialization_of_impl<std::remove_cv_t<T>, Template>;

template <typename T, template <typename...> class Template>
inline constexpr bool is_specialization_of_v = is_specialization_of<std::remove_cv_t<T>, Template>::value;

template <typename T>
using is_optional = is_specialization_of<T, std::optional>;

template <typename T>
inline constexpr bool is_optional_v = is_optional<T>::value;

template <typename T>
struct has_begin_end : std::bool_constant<detail::has_begin_end_impl<T>::value> {};

template <typename T>
inline constexpr bool has_begin_end_v = has_begin_end<T>::value;

template <typename T>
struct has_key_value_pair : std::bool_constant<detail::has_key_value_pair_impl<T>::value> {};

template <typename T>
inline constexpr bool has_key_value_pair_v = has_key_value_pair<T>::value;

// is_explicitly_convertible

template <typename From, typename To>
struct is_explicitly_convertible {
    static constexpr bool value = std::is_constructible_v<To, From> && !std::is_convertible_v<From, To>;
};

template <typename From, typename To>
inline constexpr bool is_explicitly_convertible_v = is_explicitly_convertible<From, To>::value;

// is_empty

template <typename... Args>
struct is_empty : std::bool_constant<sizeof...(Args) == 0> {};

template <typename... Args>
inline constexpr bool is_empty_v = is_empty<Args...>::value;

// are_same

template <typename T, typename...>
struct are_same : std::true_type {};

template <typename T, typename U, typename... Args>
struct are_same<T, U, Args...> : std::bool_constant<std::is_same<T, U>::value && are_same<T, Args...>::value> {};

template <typename... Args>
inline constexpr bool are_same_v = are_same<Args...>::value;

// enums

template <typename...>
struct are_enum : std::true_type {};

template <typename E>
struct are_enum<E> : std::is_enum<E> {};

template <typename E, typename... Args>
struct are_enum<E, Args...> : std::bool_constant<std::is_enum_v<E> && are_enum<Args...>::value> {};

template <typename E, typename... Args>
inline constexpr bool are_enum_v = are_enum<E, Args...>::value;

// unqualified

template <typename T>
using unqualified_t = std::remove_cv_t<std::remove_reference_t<T>>;

// operators

template <typename T>
using not_t = std::bool_constant<!T::value>;

template <typename T>
inline constexpr bool not_v = not_t<T>::value;

template <typename Condition, typename Then, typename Else>
using if_t = std::conditional_t<Condition::value, Then, Else>;

template <typename Condition, typename Then, typename Else>
inline constexpr bool if_v = if_t<Condition, Then, Else>::value;

template <typename... Args>
struct and_t : std::bool_constant<true> {};

template <typename T, typename... Args>
struct and_t<T, Args...> : if_t<T, and_t<Args...>, std::bool_constant<false>> {};

template <typename... Args>
inline constexpr bool and_v = and_t<Args...>::value;

template <typename... Args>
struct or_t : std::bool_constant<false> {};

template <typename T, typename... Args>
struct or_t<T, Args...> : if_t<T, std::bool_constant<true>, or_t<Args...>> {};

template <typename... Args>
inline constexpr bool or_v = or_t<Args...>::value;

template <typename... Args>
using enable_if_all_t = std::enable_if_t<and_t<Args...>::value, int>;

template <typename... Args>
using enable_if_any_t = std::enable_if_t<or_t<Args...>::value, int>;

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
using result_of_t = typename result_of<T>::type;

template <typename T>
struct functor_traits : public functor_traits<decltype(&T::operator())> {};

template <typename T, typename R, typename... Args>
struct functor_traits<R (T::*)(Args...) const> {
    static constexpr auto arity = sizeof...(Args);

    using return_type = R;
    using args_type = std::tuple<Args...>;
    using function_type = std::function<R(Args...)>;

    template <std::size_t N>
    using arg_type = typename std::tuple_element<N, args_type>::type;
};

template <typename R, typename... Args>
struct functor_traits<R(Args...)> {
    static constexpr auto arity = sizeof...(Args);

    using return_type = R;
    using args_type = std::tuple<Args...>;
    using function_type = std::function<R(Args...)>;

    template <std::size_t N>
    using arg_type = typename std::tuple_element<N, args_type>::type;
};

} // namespace meta

using meta::is_bool;
using meta::is_bool_v;
using meta::is_integer;
using meta::is_integer_v;
using meta::is_numeric;
using meta::is_numeric_v;
using meta::is_optional;
using meta::is_optional_v;
using meta::is_specialization_of;
using meta::is_specialization_of_v;
using meta::is_streamable;
using meta::is_streamable_v;

using meta::has_begin_end;
using meta::has_begin_end_v;
using meta::has_key_value_pair;
using meta::has_key_value_pair_v;

using meta::are_same;
using meta::are_same_v;
using meta::is_empty;
using meta::is_empty_v;

using meta::are_enum;
using meta::are_enum_v;

using meta::unqualified_t;

using meta::and_t;
using meta::and_v;
using meta::if_t;
using meta::if_v;
using meta::not_t;
using meta::not_v;
using meta::or_t;
using meta::or_v;

using meta::enable_if_all_t;
using meta::enable_if_any_t;

using meta::disable_if;
using meta::disable_if_t;

using meta::functor_traits;
using meta::result_of_t;

} // namespace shard
