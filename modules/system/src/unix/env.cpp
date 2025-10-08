// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <optional>
#include <string>

extern char** environ; /* NOLINT */

namespace impl {

char** variables = ::environ; /* NOLINT */

bool has_env(const std::string& key) noexcept {
    return ::getenv(key.c_str()) != nullptr;
}

std::optional<std::string> get_env(const std::string& key) noexcept {
    if (auto value = ::getenv(key.c_str())) {
        return std::string(value);
    }
    return std::nullopt;
}

bool set_env(const std::string& key, const std::string& value) noexcept {
    return ::setenv(key.c_str(), value.c_str(), 1) == 0;
}

bool unset_env(const std::string& key) noexcept {
    return ::unsetenv(key.c_str()) == 0;
}

} // namespace impl
