// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include "shard/system/platform.hpp"

#include <cstdlib>
#include <mutex>

#if defined(SHARD_WINDOWS)
extern char** _environ;
#define g_environ _environ
#else
extern char** environ;
#define g_environ environ
#endif

namespace shard {
namespace system {
namespace env {

static std::mutex g_mutex;
static std::map<std::string, std::string> g_variables;

bool has(const std::string& key) {
    std::lock_guard<std::mutex> lock(g_mutex);
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
    } catch (...) { return false; }
}

optional<std::string> get(const std::string& key) {
    std::lock_guard<std::mutex> lock(g_mutex);
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
    return nullopt;
}

bool set(const std::string& key, const std::string& value, std::string* old_value) {
    if (old_value) {
        if (auto v = get(key)) {
            *old_value = *v;
        }
    }
    std::lock_guard<std::mutex> lock(g_mutex);
    try {
#if defined(SHARD_WINDOWS)
        return ::_putenv_s(key.c_str(), value.c_str()) == 0;
#else
        return ::setenv(key.c_str(), value.c_str(), 1) == 0;
#endif
    } catch (...) { return false; }
}

bool unset(const std::string& key, std::string* old_value) {
    if (old_value) {
        if (auto v = get(key)) {
            *old_value = *v;
        }
    }
    std::lock_guard<std::mutex> lock(g_mutex);
    try {
#if defined(SHARD_WINDOWS)
        return ::_putenv_s(key.c_str(), "") == 0;
#else
        return ::unsetenv(key.c_str()) == 0;
#endif
    } catch (...) { return false; }
}

const std::map<std::string, std::string>& vars() {
    auto i = 1;
    auto s = *g_environ;
    g_variables.clear();
    for (; s; i++) {
        std::string str(s);
        auto eq = str.find_first_of('=');
        if (eq == std::string::npos) {
            continue;
        }
        g_variables[str.substr(0, eq)] = str.substr(eq + 1);
        // get the next line
        s = *(g_environ + i);
    }
    return g_variables;
}

} // namespace env
} // namespace system
} // namespace shard
