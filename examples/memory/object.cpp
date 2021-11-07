// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/memory/object.hpp>

#include <iostream>

class widget : public shard::object {
public:
    widget() = default;

    ~widget() { std::cout << "~widget()\n"; }

    explicit widget(int id) : m_id(id) {}

    int id() const { return m_id; }

private:
    int m_id = 0;
};

int main(int /* argc */, char* /* argv */[]) {
    widget* raw;

    {
        shard::object_ptr<widget> w;
        std::cout << w.get() << '\n';

        w = new widget(42);
        std::cout << "after new:  " << w->ref_count() << '\n';

        {
            auto copied = w;
            std::cout << "after copy: " << w->ref_count() << '\n';

            auto moved = std::move(copied);
            std::cout << "after move: " << w->ref_count() << '\n';
            std::cout << "a: " << w->id() << '\n';
        }
        std::cout << "after dtor: " << w->ref_count() << '\n';

        raw = w.get();
    }
    std::cout << "after dtor: " << raw->ref_count() << '\n';

    return 0;
}
