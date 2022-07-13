// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include "shard/system/platform.hpp"

#include <cstdlib>
#include <mutex>

#if defined(SHARD_WINDOWS)
extern char** _environ;
#define shard_environ _environ
#else
extern char** environ;
#define shard_environ environ
#endif

namespace shard::system::env {
namespace detail {

inline std::mutex& env_mutex() {
    static std::mutex mutex;
    return mutex;
}

inline std::map<std::string, std::string>& env_variables() {
    static std::map<std::string, std::string> variables;
    return variables;
}

} // namespace detail

inline bool has(const std::string& key) {
    std::lock_guard<std::mutex> lock(detail::env_mutex());
    try {
#if defined(SHARD_WINDOWS)
        std::size_t size;
        char buffer[MAX_ENV];
        if (::getenv_s(&size, buffer, key.c_str()) == 0) {
            return size != 0;
        }
        return false;
#else
        return ::getenv(key.c_str()) != nullptr;
#endif
    } catch (...) {
        return false;
    }
}

inline std::optional<std::string> get(const std::string& key) {
    std::lock_guard<std::mutex> lock(detail::env_mutex());
    try {
#if defined(SHARD_WINDOWS)
        std::size_t size;
        char buffer[MAX_ENV];
        if (::getenv_s(&size, buffer, key.c_str()) == 0) {
            return buffer;
        }
#else
        if (auto value = ::getenv(key.c_str())) {
            return value;
        }
#endif
    } catch (...) {
        // fall through
    }
    return std::nullopt;
}

inline bool set(const std::string& key, const std::string& value, std::string* old_value) {
    if (old_value) {
        if (auto v = get(key)) {
            *old_value = *v;
        }
    }
    std::lock_guard<std::mutex> lock(detail::env_mutex());
    try {
#if defined(SHARD_WINDOWS)
        return ::_putenv_s(key.c_str(), value.c_str()) == 0;
#else
        return ::setenv(key.c_str(), value.c_str(), 1) == 0;
#endif
    } catch (...) {
        return false;
    }
}

inline bool unset(const std::string& key, std::string* old_value) {
    if (old_value) {
        if (auto v = get(key)) {
            *old_value = *v;
        }
    }
    std::lock_guard<std::mutex> lock(detail::env_mutex());
    try {
#if defined(SHARD_WINDOWS)
        return ::_putenv_s(key.c_str(), "") == 0;
#else
        return ::unsetenv(key.c_str()) == 0;
#endif
    } catch (...) {
        return false;
    }
}

inline const std::map<std::string, std::string>& vars() {
    auto i = 1;
    auto s = *shard_environ;
    detail::env_variables().clear();
    for (; s; i++) {
        std::string str(s);
        auto eq = str.find_first_of('=');
        if (eq == std::string::npos) {
            continue;
        }
        detail::env_variables()[str.substr(0, eq)] = str.substr(eq + 1);
        // get the next line
        s = *(shard_environ + i);
    }
    return detail::env_variables();
}

} // namespace shard::system::env
