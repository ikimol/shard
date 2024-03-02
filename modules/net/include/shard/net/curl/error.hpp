// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <curl/curl.h>

namespace shard::net::curl {

class error {
public:
    /// Create a wrapper for an error code
    explicit error(CURLcode code)
    : m_code(code) {}

    /// Get the error code
    CURLcode code() const { return m_code; }

    /// Get the error message
    const char* message() const { return curl_easy_strerror(m_code); }

private:
    CURLcode m_code;
};

} // namespace shard::net::curl
