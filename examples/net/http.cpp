// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/net/http.hpp>
#include <shard/string/replace.hpp>
#include <shard/string/trim.hpp>

#include <iostream>

namespace http = shard::net::http;

std::optional<std::string> data_to_string(const shard::memory::dynamic_data& data) {
    if (data.bytes()) {
        std::string s(reinterpret_cast<const char*>(data.bytes()), data.size());
        shard::trim(s);
        shard::replace_all(s, "\r\n", "\n");
        return s;
    }
    return std::nullopt;
}

int main(int /* argc */, char* /* argv */[]) {
    http::client client;
    auto request = http::request_builder()
                       .with_url("https://www.example.com")
                       .with_method(http::request::method_get)
                       .make_cancellable()
                       .build();
    auto future = client.send_request(std::move(request));
    future.get()
        .and_then([](http::response&& r) -> http::result {
            if (auto headers = data_to_string(r.header())) {
                std::cout << *headers << '\n';
            }
            return std::move(r);
        })
        .or_else([](shard::net::curl::error&& e) -> http::result {
            std::cerr << "Request failed: " << e.message() << '\n';
            return shard::unexpected(e);
        });
    return 0;
}