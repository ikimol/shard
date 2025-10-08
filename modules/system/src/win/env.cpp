// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <optional>
#include <string>

#include <stdlib.h>

namespace impl {

char** variables = _environ; /* NOLINT */

bool has_env(const std::string& key) noexcept {
    std::size_t size;
    if (char buffer[_MAX_ENV]; ::getenv_s(&size, buffer, key.c_str()) == 0) {
        return size != 0;
    }
    return false;
}

std::optional<std::string> get_env(const std::string& key) noexcept {
    std::size_t size;
    if (char buffer[_MAX_ENV]; ::getenv_s(&size, buffer, key.c_str()) == 0) {
        return std::string(buffer, size);
    }
    return std::nullopt;
}

bool set_env(const std::string& key, const std::string& value) noexcept {
    return ::_putenv_s(key.c_str(), value.c_str()) == 0;
}

bool unset_env(const std::string& key) noexcept {
    return ::_putenv_s(key.c_str(), "") == 0;
}

} // namespace impl
