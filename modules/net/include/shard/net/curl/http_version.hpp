// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <curl/curl.h>

namespace shard::net::curl {

enum class http_version_t {
    v_1_0 = CURL_HTTP_VERSION_1_0,
    v_1_1 = CURL_HTTP_VERSION_1_1,
    v_2 = CURL_HTTP_VERSION_2,
#if LIBCURL_VERSION_NUM >= 0x074200
    v_3 = CURL_HTTP_VERSION_3,
#endif
};

} // namespace shard::net::curl
