// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include <shard/net/http.hpp>
#include <shard/string/replace.hpp>
#include <shard/string/trim.hpp>

#include <nlohmann/json.hpp>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using json = nlohmann::json;

namespace http = shard::net::http;

enum class error_code {
    curl_error,
    internal_error,
    json_parse_error,
};

namespace api {

struct time_result {
    std::string client_ip;
    std::string datetime;
    int day_of_week = 0;
    bool dst = false;
    int dst_offset = 0;
    std::string timezone;
    std::time_t unixtime = 0;
    std::string utc_datetime;
};

time_result parse_time_result(const json& in_json) {
    time_result result;
    in_json["client_ip"].get_to(result.client_ip);
    in_json["datetime"].get_to(result.datetime);
    in_json["day_of_week"].get_to(result.day_of_week);
    in_json["dst"].get_to(result.dst);
    in_json["dst_offset"].get_to(result.dst_offset);
    in_json["timezone"].get_to(result.timezone);
    in_json["unixtime"].get_to(result.unixtime);
    in_json["utc_datetime"].get_to(result.utc_datetime);
    return result;
}

} // namespace api

namespace convert {

error_code to_error_code(const shard::net::curl::error&) {
    return error_code::curl_error;
}

shard::expected<std::string, error_code> to_string(const http::response& response) {
    if (response.data().bytes()) {
        std::string data(reinterpret_cast<const char*>(response.data().bytes()), response.data().size());
        shard::trim(data);
        shard::replace_all(data, "\r\n", "\n");
        return data;
    }
    return shard::unexpected(error_code::internal_error);
}

shard::expected<json, error_code> to_json(const std::string& raw_json) {
    try {
        return json::parse(raw_json);
    } catch (...) {
        return shard::unexpected(error_code::json_parse_error);
    }
}

} // namespace convert

shard::expected<api::time_result, error_code> get_time(http::client& client) {
    auto request = http::request_builder()
                       .with_url("https://worldtimeapi.org/api/ip")
                       .with_method(http::request::method_get)
                       .build();
    return client.send_request(std::move(request))
        .get()
        .transform_error(convert::to_error_code)
        .and_then(convert::to_string)
        .and_then(convert::to_json)
        .transform(api::parse_time_result);
}

int main(int /* argc */, char* /* argv */[]) {
    http::client client;

    if (auto time = get_time(client)) {
        std::tm t = {};
        std::istringstream(time->utc_datetime) >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");
        t.tm_wday = time->day_of_week;
        t.tm_isdst = time->dst;
        std::cout << std::put_time(&t, "%c") << '\n';
    }
    return 0;
}
