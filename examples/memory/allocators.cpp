// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/memory/allocators/free_list_allocator.hpp>
#include <shard/memory/allocators/heap_allocator.hpp>
#include <shard/memory/allocators/linear_allocator.hpp>
#include <shard/memory/allocators/pool_allocator.hpp>
#include <shard/memory/allocators/proxy_allocator.hpp>
#include <shard/memory/allocators/static_allocator.hpp>

#include <cstring>
#include <iostream>

#define BUFFER_SIZE 512

// statically allocated buffer
char buffer[BUFFER_SIZE];

struct widget {
    explicit widget(std::string name)
    : name(std::move(name)) {}

    ~widget() { std::cout << name << "::~widget()\n"; }

    std::string name;
};

template <typename A>
void print_divider(const A&) {
    static const std::string divider(25, '~');
    std::cout << divider << '\n';
}

template <>
void print_divider(const shard::proxy_allocator& a) {
    auto name_length = std::strlen(a.name());
    const std::string divider(25 - name_length - 1, '~');
    std::cout << divider << ' ' << a.name() << '\n';
}

template <typename A>
void print_allocator_stats(const A& a) {
    print_divider(a);
    std::cout << "size: " << a.size() << '\n';
    std::cout << "used_memory: " << a.used_memory() << '\n';
    std::cout << "allocation_count: " << a.allocation_count() << '\n';
}

int main(int /* argc */, char* /* argv */[]) {
    std::cout << "sizeof(widget): " << sizeof(widget) << '\n';

    // free list allocator
    {
        shard::free_list_allocator a(buffer, BUFFER_SIZE);
        auto w = shard::new_object<widget>(a, "widget-0");
        print_allocator_stats(a);
        shard::delete_object(a, w);
    }

    // heap allocator
    {
        shard::heap_allocator a;
        auto w = shard::new_object<widget>(a, "widget-1");
        print_allocator_stats(a);
        shard::delete_object(a, w);
    }

    // linear allocator
    {
        shard::linear_allocator a(buffer, BUFFER_SIZE);
        auto w = shard::new_object<widget>(a, "widget-2");
        print_allocator_stats(a);
        // manually call destructor
        w->~widget();
        a.clear();
    }

    // pool allocator
    {
        shard::pool_allocator<widget> a(buffer, BUFFER_SIZE);
        auto w = shard::new_object<widget>(a, "widget-3");
        print_allocator_stats(a);
        shard::delete_object(a, w);
    }

    // proxy allocator
    {
        shard::free_list_allocator a(buffer, BUFFER_SIZE);
        shard::proxy_allocator proxy(a, "proxy");
        auto w = shard::new_object<widget>(proxy, "widget-4");
        print_allocator_stats(proxy);
        shard::delete_object(proxy, w);
    }

    // static allocator
    {
        shard::static_allocator<BUFFER_SIZE> a;
        auto w = shard::new_object<widget>(a, "widget-5");
        print_allocator_stats(a);
        // manually call destructor
        w->~widget();
        a.clear();
    }

    std::memset(buffer, '\0', BUFFER_SIZE);

    return 0;
}