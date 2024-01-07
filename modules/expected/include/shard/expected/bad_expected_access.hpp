// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#pragma once

#include <exception>
#include <utility>

namespace shard {

template <typename E>
class bad_expected_access;

template <>
class bad_expected_access<void> : public std::exception {
public:
    const char* what() const noexcept override { return "bad access to shard::expected"; }

protected:
    bad_expected_access() noexcept = default;
    bad_expected_access(const bad_expected_access&) = default;
    bad_expected_access(bad_expected_access&&) = default;
    ~bad_expected_access() override = default;
    bad_expected_access& operator=(const bad_expected_access&) = default;
    bad_expected_access& operator=(bad_expected_access&&) = default;
};

template <typename E>
class bad_expected_access : public bad_expected_access<void> {
public:
    explicit bad_expected_access(E error)
    : m_error(std::move(error)) {}

    const E& error() noexcept { return m_error; }

private:
    E m_error;
};

} // namespace shard
