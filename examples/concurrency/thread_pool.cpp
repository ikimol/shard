// Copyright (c) 2022 Miklos Molnar. All rights reserved.

#include <shard/concurrency/thread_pool.hpp>

#include <iostream>

static void foo() {
    std::cout << "foo()\n";
}

static void bar(int i) {
    std::cout << "bar(" << i << ")\n";
}

struct widget {
    explicit widget(const char* str) : str(str) {}

    void baz() const {
        std::cout << "widget::baz('" << str << "')\n";
    }

    const char* str;
};

int main() {
    shard::thread_pool threads(8);

    threads.run([] { std::cout << "lambda\n"; });
    threads.run(&foo);
    threads.run(&bar, 42);

    widget w("quack");
    threads.run(&widget::baz, std::ref(w));

    std::this_thread::sleep_for(std::chrono::milliseconds (50));

    return 0;
}
