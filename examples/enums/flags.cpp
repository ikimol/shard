// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/enums/flags.hpp>

#include <iostream>

class file {
public:
    enum open_mode_flags : unsigned int {
        unknown = 0,
        read_only = 1 << 0,
        write_only = 1 << 1,
        read_write = read_only | write_only,
        truncate = 1 << 2,
        append = 1 << 3,
        binary = 1 << 4
    };
    SHARD_DECLARE_FLAGS(open_mode, open_mode_flags)

public:
    explicit file(open_mode mode)
    : m_mode(mode) {}

    void open() {
        std::cout << "read_only:  " << std::boolalpha << m_mode.test(open_mode_flags::read_only) << '\n';
        std::cout << "read_write: " << std::boolalpha << m_mode.test(open_mode_flags::read_write) << '\n';
        std::cout << "truncate:   " << std::boolalpha << m_mode.test(open_mode_flags::truncate) << '\n';
        std::cout << "append:     " << std::boolalpha << m_mode.test(open_mode_flags::append) << '\n';
        std::cout << "binary:     " << std::boolalpha << m_mode.test(open_mode_flags::binary) << '\n';
    }

private:
    open_mode m_mode;
};

SHARD_DECLARE_FLAG_OPERATORS(file::open_mode)

int main(int /* argc */, char* /* argv */[]) {
    file f(file::binary | file::read_only);
    f.open();

    file::open_mode flags(file::truncate);
    flags |= file::write_only;
    std::cout << std::hex << flags.value() << '\n';

    return 0;
}