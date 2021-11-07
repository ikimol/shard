// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/signal.hpp>

#include <cassert>
#include <iostream>

struct widget {
    explicit widget(const char* name) : name(name) {}

    void foo() { std::cout << name << '\n'; }

    const char* name;
};

struct receiver {
    void on_event() { received = true; }

    bool received = false;
};

int main(int argc, char* argv[]) {
    // create a signal with the list of argument types
    shard::signal<int> event;

    // connect a lambda
    auto c = event.connect([](int i) { std::cout << i << '\n'; });

    // emit the signal
    event.emit(1);

    // disable the connected slot
    c.set_enabled(false);

    // connect a member function
    receiver r;
    event.connect(&receiver::on_event, &r);

    // can also use the call operator
    event(2);

    // verify that the member function was called
    assert(r.received);

    // re-enable the connected slot
    c.set_enabled(true);

    {
        widget w("foo");

        // bind a function that takes no arguments
        auto lambda = [obj = &w] {
            obj->foo();
        };

        // the slot will be disconnected at the end of the scope
        shard::scoped_connection sc = event.connect(lambda);
        event.emit(3);
    }

    event.emit(4);

    return 0;
}