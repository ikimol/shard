// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_NET_CURL_HANDLE_HPP
#define SHARD_NET_CURL_HANDLE_HPP

#include "shard/net/curl/detail/type_converter.hpp"
#include "shard/net/curl/info.hpp"
#include "shard/net/curl/option.hpp"

#include <curl/curl.h>

#include <optional>
#include <stdexcept>

namespace shard::net::curl {

class handle {
public:
    /// Default constructor
    handle() : m_curl(curl_easy_init()) {
        if (!m_curl) {
            throw std::runtime_error("failed to initialize curl");
        }
    }

    /// Destructor
    ~handle() {
        if (m_curl) {
            curl_easy_cleanup(m_curl);
        }
    }

    /// Perform the request
    void perform() { m_code = curl_easy_perform(m_curl); }

    /// Convenience method to check if the error code is ok
    bool is_ok() const { return m_code == CURLE_OK; }

    /// Get the last error message
    CURLcode error_code() const { return m_code; }

    /// Get the last error message
    const char* error_msg() const { return curl_easy_strerror(m_code); }

    /// Reset the curl handle to its default settings
    void reset() { curl_easy_reset(m_curl); }

    /// Set a curl option in a type-safe manner
    template <CURLoption t_option>
    bool set_option(const option_t<t_option>& value) {
        auto tmp = detail::type_converter<option_t<t_option>>::to_curl(value);
        auto code = curl_easy_setopt(m_curl, t_option, tmp);
        return code == CURLE_OK;
    }

    /// Set a curl option without any type checks
    template <typename T>
    bool set_option_raw(CURLoption option, T&& value) {
        return curl_easy_setopt(m_curl, option, std::forward<T>(value));
    }

    /// Get a piece of curl info in a type-safe manner
    template <CURLINFO t_info>
    bool get_info(info_t<t_info>& out) {
        detail::curl_t<info_t<t_info>> tmp;
        auto code = curl_easy_getinfo(m_curl, t_info, &tmp);
        out = tmp;
        return code == CURLE_OK;
    }

    /// Get a piece of curl info in a type-safe manner
    template <CURLINFO t_info>
    std::optional<info_t<t_info>> get_info() {
        info_t<t_info> value {};
        if (get_info<t_info>(value)) {
            return value;
        }
        return std::nullopt;
    }

    /// URL encode the given string
    std::string escape(const std::string& string) {
        auto encoded = curl_easy_escape(m_curl, string.c_str(), (int)(string.size()));
        std::string result(encoded);
        curl_free(encoded);
        return result;
    }

    /// Get the underlying curl handle
    CURL* get() const { return m_curl; }

private:
    CURL* m_curl;
    CURLcode m_code = CURL_LAST;
};

} // namespace shard::net::curl

#endif // SHARD_NET_CURL_HANDLE_HPP
