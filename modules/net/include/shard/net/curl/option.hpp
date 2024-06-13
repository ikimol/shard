// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/net/curl/http_version.hpp"
#include "shard/net/curl/proxy.hpp"
#include "shard/net/curl/slist.hpp"
#include "shard/net/url.hpp"

#include <curl/curl.h>

#include <cstdio>
#include <optional>

namespace shard::net::curl {

// template declaration

template <CURLoption>
struct option;

// clang-format off

template <> struct option<CURLOPT_URL> { using type = url; };

template <> struct option<CURLOPT_HTTP_VERSION> { using type = http_version_t; };

template <> struct option<CURLOPT_FOLLOWLOCATION> { using type = bool; };
template <> struct option<CURLOPT_CONNECT_ONLY> { using type = bool; };

// HTTP GET request
template <> struct option<CURLOPT_HTTPGET> { using type = bool; };

// HTTP POST request
template <> struct option<CURLOPT_POST> { using type = bool; };
template <> struct option<CURLOPT_POSTFIELDSIZE> { using type = std::size_t; };
template <> struct option<CURLOPT_POSTFIELDS> { using type = std::optional<std::string>; };
template <> struct option<CURLOPT_COPYPOSTFIELDS> { using type = std::optional<std::string>; };

// HTTP HEAD request
template <> struct option<CURLOPT_NOBODY> { using type = bool; };

// HTTP PUT request
template <> struct option<CURLOPT_PUT> { using type = bool; };

// custom HTTP request
template <> struct option<CURLOPT_CUSTOMREQUEST> { using type = std::optional<std::string>; };

template <> struct option<CURLOPT_HTTPHEADER> { using type = slist; };

template <> struct option<CURLOPT_TIMEOUT> { using type = std::size_t; };
template <> struct option<CURLOPT_TIMEOUT_MS> { using type = std::size_t; };
template <> struct option<CURLOPT_CONNECTTIMEOUT> { using type = std::size_t; };
template <> struct option<CURLOPT_CONNECTTIMEOUT_MS> { using type = std::size_t; };

template <> struct option<CURLOPT_SSL_VERIFYPEER> { using type = bool; };
template <> struct option<CURLOPT_SSL_VERIFYHOST> { using type = long; };
template <> struct option<CURLOPT_CAINFO> { using type = std::string; };

template <> struct option<CURLOPT_PROXY> { using type = std::optional<std::string>; };
template <> struct option<CURLOPT_PROXYPORT> { using type = std::size_t; };
template <> struct option<CURLOPT_PROXYTYPE> { using type = proxy_t; };
template <> struct option<CURLOPT_HTTPPROXYTUNNEL> { using type = bool; };

template <> struct option<CURLOPT_WRITEFUNCTION> { using type = curl_write_callback; };
template <> struct option<CURLOPT_WRITEDATA> { using type = void*; };

template <> struct option<CURLOPT_READFUNCTION> { using type = curl_read_callback; };
template <> struct option<CURLOPT_READDATA> { using type = void*; };

template <> struct option<CURLOPT_HEADERFUNCTION> { using type = curl_write_callback; };
template <> struct option<CURLOPT_HEADERDATA> { using type = void*; };

template <> struct option<CURLOPT_NOPROGRESS> { using type = bool; };
template <> struct option<CURLOPT_PROGRESSFUNCTION> { using type = curl_progress_callback; };
template <> struct option<CURLOPT_XFERINFOFUNCTION> { using type = curl_xferinfo_callback; };
template <> struct option<CURLOPT_XFERINFODATA> { using type = void*; };

template <> struct option<CURLOPT_STDERR> { using type = FILE*; };
template <> struct option<CURLOPT_VERBOSE> { using type = bool; };
template <> struct option<CURLOPT_DEBUGFUNCTION> { using type = curl_debug_callback; };
template <> struct option<CURLOPT_DEBUGDATA> { using type = void*; };

template <> struct option<CURLOPT_PRIVATE> { using type = void*; };

// clang-format on

// convenience typedef

template <CURLoption Option>
using option_t = typename option<Option>::type;

} // namespace shard::net::curl
