// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#include <shard/memory/unowned_ptr.hpp>

#include <iostream>

class widget {
public:
    widget() = default;

    ~widget() { std::cout << "~widget()\n"; }

    explicit widget(int id)
    : m_id(id) {}

    int id() const { return m_id; }

    void set_id(int id) { m_id = id; }

private:
    int m_id = 0;
};

static void delete_widget(const widget* w) {
    delete w;
}

int main(int /* argc */, char* /* argv */[]) {
    auto w = new widget(42);
    {
        shard::managed_ptr<widget, delete_widget> managed(w);
        {
            shard::unowned_ptr<widget> unowned(managed);
            unowned->set_id(21);
            std::cout << "before unowned destructor\n";
        }
        std::cout << "before managed destructor\n";
    }
    std::cout << "after managed destructor\n";

    return 0;
}