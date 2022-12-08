// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_NET_CURL_INFO_HPP
#define SHARD_NET_CURL_INFO_HPP

#include <curl/curl.h>

#include <string>

namespace shard::net::curl {

// template declaration

template <CURLINFO>
struct info;

// clang-format off

template <> struct info<CURLINFO_RESPONSE_CODE> { using type = long; };
template <> struct info<CURLINFO_CONTENT_TYPE> { using type = std::string; };

template <> struct info<CURLINFO_TOTAL_TIME> { using type = double; };
template <> struct info<CURLINFO_NAMELOOKUP_TIME> { using type = double; };
template <> struct info<CURLINFO_CONNECT_TIME> { using type = double; };
template <> struct info<CURLINFO_APPCONNECT_TIME> { using type = double; };
template <> struct info<CURLINFO_PRETRANSFER_TIME> { using type = double; };
template <> struct info<CURLINFO_STARTTRANSFER_TIME> { using type = double; };
template <> struct info<CURLINFO_REDIRECT_TIME> { using type = double; };

#if LIBCURL_VERSION_NUM >= 0x073D00
template <> struct info<CURLINFO_TOTAL_TIME_T> { using type = curl_off_t; };
template <> struct info<CURLINFO_NAMELOOKUP_TIME_T> { using type = curl_off_t; };
template <> struct info<CURLINFO_CONNECT_TIME_T> { using type = curl_off_t; };
template <> struct info<CURLINFO_APPCONNECT_TIME_T> { using type = curl_off_t; };
template <> struct info<CURLINFO_PRETRANSFER_TIME_T> { using type = curl_off_t; };
template <> struct info<CURLINFO_STARTTRANSFER_TIME_T> { using type = curl_off_t; };
template <> struct info<CURLINFO_REDIRECT_TIME_T> { using type = curl_off_t; };
#endif

template <> struct info<CURLINFO_FILETIME> { using type = long; };

#if LIBCURL_VERSION_NUM >= 0x073B00
template <> struct info<CURLINFO_FILETIME_T> { using type = curl_off_t; };
#endif

template <> struct info<CURLINFO_REDIRECT_COUNT> { using type = long; };
template <> struct info<CURLINFO_REDIRECT_URL> { using type = std::string; };

template <> struct info<CURLINFO_CONTENT_LENGTH_UPLOAD> { using type = double; };
template <> struct info<CURLINFO_CONTENT_LENGTH_DOWNLOAD> { using type = double; };
template <> struct info<CURLINFO_SIZE_UPLOAD> { using type = double; };
template <> struct info<CURLINFO_SIZE_DOWNLOAD> { using type = double; };

#if LIBCURL_VERSION_NUM >= 0x073700
template <> struct info<CURLINFO_CONTENT_LENGTH_UPLOAD_T> { using type = curl_off_t; };
template <> struct info<CURLINFO_CONTENT_LENGTH_DOWNLOAD_T> { using type = curl_off_t; };
template <> struct info<CURLINFO_SIZE_UPLOAD_T> { using type = curl_off_t; };
template <> struct info<CURLINFO_SIZE_DOWNLOAD_T> { using type = curl_off_t; };
#endif

template <> struct info<CURLINFO_SPEED_UPLOAD> { using type = double; };
template <> struct info<CURLINFO_SPEED_DOWNLOAD> { using type = double; };

#if LIBCURL_VERSION_NUM >= 0x073700
template <> struct info<CURLINFO_SPEED_UPLOAD_T> { using type = curl_off_t; };
template <> struct info<CURLINFO_SPEED_DOWNLOAD_T> { using type = curl_off_t; };
#endif

template <> struct info<CURLINFO_HEADER_SIZE> { using type = long; };
template <> struct info<CURLINFO_REQUEST_SIZE> { using type = long; };

// clang-format on

// convenience typedef

template <CURLINFO t_info>
using info_t = typename info<t_info>::type;

} // namespace shard::net::curl

#endif // SHARD_NET_CURL_INFO_HPP
