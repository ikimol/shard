// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/net/curl.hpp>

#include <iostream>

static std::size_t empty_writer(char* /* data */, std::size_t size, std::size_t nmemb, void* /* user_data */) {
    return size * nmemb;
}

namespace curl = shard::net::curl;

int main(int /* argc */, char* /* argv */[]) {
    shard::url url("https://www.example.com");

    curl::handle handle;

    // set options in a type safe manner
    handle.set_option<CURLOPT_URL>(url);
    handle.set_option<CURLOPT_WRITEFUNCTION>(empty_writer);

    handle.perform();

    // check return code & print error
    if (handle.error_code() != CURLE_OK) {
        std::cerr << handle.error_msg() << '\n';
        return 1;
    }

    // get info from the response
    std::cout << "response code: " << *handle.get_info<CURLINFO_RESPONSE_CODE>() << '\n';
    std::cout << "download size: " << *handle.get_info<CURLINFO_SIZE_DOWNLOAD>() << '\n';
    std::cout << "download speed: " << *handle.get_info<CURLINFO_SPEED_DOWNLOAD>() << '\n';

    return 0;
}
