// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <curl/curl.h>

namespace shard::net::curl {

enum class proxy_t {
    http = CURLPROXY_HTTP,
    http_1_0 = CURLPROXY_HTTP_1_0,
    https = CURLPROXY_HTTPS,
#if LIBCURL_VERSION_NUM >= 0x080100
    https_2 = CURLPROXY_HTTPS2,
#endif
    socks_4 = CURLPROXY_SOCKS4,
    socks_5 = CURLPROXY_SOCKS5,
    socks_4_a = CURLPROXY_SOCKS4A,
    socks_5_hostname = CURLPROXY_SOCKS5_HOSTNAME,
};

} // namespace shard::net::curl
