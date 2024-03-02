// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include <shard/expected.hpp>

#include <iostream>
#include <string>
#include <system_error>

#include <expected>

shard::expected<std::string, std::error_condition> read_input() {
    std::string s;
    std::cin >> s;
    if (std::cin.fail()) {
        return shard::unexpected(std::make_error_condition(std::io_errc::stream));
    }
    return s;
}

shard::expected<int, std::error_condition> to_int(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (std::exception&) {
        return shard::unexpected(std::make_error_condition(std::errc::argument_out_of_domain));
    }
}

int add_ten(int v) {
    return v + 10;
}

shard::expected<int, std::error_condition> log_error(const std::error_condition& ec) {
    std::cerr << "operation failed: " << ec.message() << '\n';
    return shard::unexpected(ec);
}

int main() {
    auto result = read_input()
                      .and_then(to_int)    // invoked if the expected contains a value
                      .transform(add_ten)  // invoked if the expected contains a value
                      .or_else(log_error); // invoked if the expected contains an error
    if (result) {
        std::cout << "result: " << *result << '\n';
    }
}