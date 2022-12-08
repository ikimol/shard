// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_HTTP_STATUS_HPP
#define SHARD_HTTP_STATUS_HPP

namespace shard::net::http {

enum class status_t {
    // 2xx success
    ok = 200,
    created = 201,
    accepted = 202,
    no_content = 204,
    reset_content = 205,
    partial_content = 206,

    // 3xx redirection
    multiple_choices = 300,
    moved_permanently = 301,
    found = 302,
    see_other = 303,
    use_proxy = 305,

    // 4xx client errors
    bad_request = 400,
    unauthorized = 401,
    forbidden = 403,
    not_found = 404,
    method_not_allowed = 405,

    // 5xx server errors
    internal_server_error = 500,
    not_implemented = 501,
    bad_gateway = 502,
    service_unavailable = 503,
    gateway_timeout = 504,
    http_version_not_supported = 505,

    // 9xx custom
    unknown = 900,
};

} // namespace shard::net::http

#endif // SHARD_HTTP_STATUS_HPP
