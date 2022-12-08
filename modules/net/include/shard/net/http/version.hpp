// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_HTTP_VERSION_HPP
#define SHARD_HTTP_VERSION_HPP

#include <curl/curl.h>

namespace shard::net::http {

enum class version_t {
    http_1_0 = CURL_HTTP_VERSION_1_0,
    http_1_1 = CURL_HTTP_VERSION_1_1,
    http_2 = CURL_HTTP_VERSION_2,
#if LIBCURL_VERSION_NUM >= 0x074200
    http_3 = CURL_HTTP_VERSION_3,
#endif
};

} // namespace shard::net::http

#endif // SHARD_HTTP_VERSION_HPP
