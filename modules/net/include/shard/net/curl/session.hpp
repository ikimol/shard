// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_NET_CURL_SESSION_HPP
#define SHARD_NET_CURL_SESSION_HPP

#include <curl/curl.h>

namespace shard::net::curl {

class session {
public:
    /// Initialize the global curl session
    ///
    /// \note The global initialization is *NOT* thread-safe and should be
    /// started before any thread is created.
    explicit session(long flags = CURL_GLOBAL_DEFAULT) { curl_global_init(flags); }

    /// Clean up the global curl session
    ~session() { curl_global_cleanup(); }
};

} // namespace shard::net::curl

#endif // SHARD_NET_CURL_SESSION_HPP
