// Copyright (c) 2022 Miklos Molnar. All rights reserved.

#include <shard/net/http.hpp>
#include <shard/string/trim.hpp>

#include <iostream>

namespace http = shard::net::http;

std::optional<std::string> data_to_string(const shard::memory::data& data) {
    if (data.bytes) {
        std::string s(reinterpret_cast<char*>(data.bytes), data.size);
        shard::trim(s);
        return s;
    }
    return std::nullopt;
}

int main(int /* argc */, char* /* argv */[]) {
    http::client client;
    auto request = http::request::create("https://www.example.com", http::request::method_get);
    auto future = client.send_request(request);
    auto& response = future.get();
    if (auto headers = data_to_string(response.header())) {
        std::cout << *headers << '\n';
    }
    return 0;
}