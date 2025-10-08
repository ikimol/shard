// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include "shard/system/env.hpp"

#include <mutex>

namespace impl {

extern char** variables; /* NOLINT */

extern bool has_env(const std::string&) noexcept;
extern std::optional<std::string> get_env(const std::string&) noexcept;
extern bool set_env(const std::string&, const std::string&) noexcept;
extern bool unset_env(const std::string&) noexcept;

} // namespace impl

namespace shard::system::env {
namespace detail {

std::mutex& env_mutex() {
    static std::mutex mutex;
    return mutex;
}

std::map<std::string, std::string>& env_variables() {
    static std::map<std::string, std::string> variables;
    return variables;
}

} // namespace detail

bool has(const std::string& key) {
    std::lock_guard lock(detail::env_mutex());
    return impl::has_env(key);
}

std::optional<std::string> get(const std::string& key) {
    std::lock_guard lock(detail::env_mutex());
    return impl::get_env(key);
}

bool set(const std::string& key, const std::string& value, std::string* old_value) {
    if (old_value) {
        if (auto v = get(key)) {
            *old_value = *v;
        }
    }
    std::lock_guard lock(detail::env_mutex());
    return impl::set_env(key, value);
}

bool unset(const std::string& key, std::string* old_value) {
    if (old_value) {
        if (auto v = get(key)) {
            *old_value = *v;
        }
    }
    std::lock_guard lock(detail::env_mutex());
    return impl::unset_env(key);
}

const std::map<std::string, std::string>& vars() {
    auto i = 1;
    auto s = *impl::variables;
    detail::env_variables().clear();
    for (; s; i++) {
        std::string str(s);
        auto eq = str.find_first_of('=');
        if (eq == std::string::npos) {
            continue;
        }
        detail::env_variables()[str.substr(0, eq)] = str.substr(eq + 1);
        // get the next line
        s = *(impl::variables + i);
    }
    return detail::env_variables();
}

} // namespace shard::system::env
