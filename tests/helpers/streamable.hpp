// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_TEST_STREAMABLE_HPP
#define SHARD_TEST_STREAMABLE_HPP

#include <ostream>

namespace test {

class streamable {
public:
    streamable() = default;

    explicit streamable(const char* name) noexcept : m_name(name) {}

    const char* name() const { return m_name; }

    void set_name(const char* name) { m_name = name; }

private:
    const char* m_name = nullptr;
};

inline std::ostream& operator<<(std::ostream& os, const streamable& s) {
    os << "streamable(\"" << s.name() << "\")";
    return os;
}

} // namespace test

#endif // SHARD_TEST_STREAMABLE_HPP
