// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_NET_CURL_DETAIL_CONVERTER_HPP
#define SHARD_NET_CURL_DETAIL_CONVERTER_HPP

#include "shard/net/curl/slist.hpp"
#include "shard/net/http/version.hpp"
#include "shard/net/url.hpp"

#include <optional>
#include <string>

namespace shard::net::curl::detail {

template <typename T>
struct type_converter {
    using user_type = T;
    using curl_type = T;

    static curl_type to_curl(const user_type& value) { return value; }
};

template <>
struct type_converter<bool> {
    using user_type = bool;
    using curl_type = long;

    static curl_type to_curl(const user_type& value) { return value ? 1l : 0l; }
};

template <>
struct type_converter<std::size_t> {
    using user_type = std::size_t;
    using curl_type = long;

    static curl_type to_curl(const user_type& value) { return static_cast<curl_type>(value); }
};

template <>
struct type_converter<std::string> {
    using user_type = std::string;
    using curl_type = char*;

    static curl_type to_curl(const user_type& value) { return const_cast<char*>(value.c_str()); }
};

template <>
struct type_converter<slist> {
    using user_type = slist;
    using curl_type = curl_slist*;

    static curl_type to_curl(const user_type& value) { return value.get(); }
};

template <>
struct type_converter<url> {
    using user_type = url;
    using curl_type = char*;

    static curl_type to_curl(const user_type& value) { return const_cast<char*>(value.to_string().c_str()); }
};

template <>
struct type_converter<http::version_t> {
    using user_type = http::version_t;
    using curl_type = long;

    static curl_type to_curl(const user_type& value) { return static_cast<long>(value); }
};

template <>
struct type_converter<std::optional<std::string>> {
    using user_type = std::optional<std::string>;
    using curl_type = char*;

    static curl_type to_curl(const user_type& value) { return value ? const_cast<char*>(value->c_str()) : nullptr; }
};

// convenience typedefs

// clang-format off
template <typename T> using user_t = typename type_converter<T>::user_type;
template <typename T> using curl_t = typename type_converter<T>::curl_type;
// clang-format on

} // namespace shard::net::curl::detail

#endif // SHARD_NET_CURL_DETAIL_CONVERTER_HPP
