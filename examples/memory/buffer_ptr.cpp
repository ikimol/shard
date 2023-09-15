// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/memory/buffer_ptr.hpp>

#include <iomanip>
#include <iostream>

class widget {
public:
    widget() = default;

    ~widget() { std::cout << "~widget()\n"; }

    explicit widget(int id)
    : m_id(id) {}

    int id() const { return m_id; }

private:
    int m_id = 0;
};

template <std::size_t N>
void print(std::byte (&array)[N]) {
    for (auto i = 0ul; i < N; ++i) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << std::to_integer<unsigned int>(array[i]) << ' ';
    }
    std::cout << std::endl;
}

int main(int /* argc */, char* /* argv */[]) {
    constexpr auto size = sizeof(widget);

    std::cout << "sizeof(widget): " << size << '\n';

    std::byte buffer[size];
    {
        shard::buffer_ptr<widget> w(buffer, 42);
        std::cout << "id: " << w->id() << '\n';
    }
    print(buffer);

    return 0;
}
