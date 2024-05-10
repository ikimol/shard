// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include <shard/net/http.hpp>
#include <shard/string/replace.hpp>
#include <shard/string/trim.hpp>

#include <nlohmann/json.hpp>

#include <array>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace http = shard::net::http;

enum class error_code {
    curl_error,
    internal_error,
    token_expired,
    token_load_error,
    token_store_error,
    json_parse_error,
};

namespace api {

struct token_result {
    std::string access_token;
    std::string token_type;
    int expires_in = 0;
};

token_result parse_token_result(const json& in_json) {
    token_result result;
    in_json["access_token"].get_to(result.access_token);
    in_json["token_type"].get_to(result.token_type);
    in_json["expires_in"].get_to(result.expires_in);
    return result;
}

struct artist_result {
    std::string id;
    std::string name;
};

artist_result parse_artist_result(const json& in_json) {
    artist_result result;
    in_json["id"].get_to(result.id);
    in_json["name"].get_to(result.name);
    return result;
}

struct album_result {
    std::string id;
    std::string name;
};

album_result parse_album_result(const json& in_json) {
    album_result result;
    in_json["id"].get_to(result.id);
    in_json["name"].get_to(result.name);
    return result;
}

struct track_result {
    std::string id;
    std::string name;
    std::optional<std::string> preview_url;
};

track_result parse_track_result(const json& in_json) {
    track_result result;
    in_json["id"].get_to(result.id);
    in_json["name"].get_to(result.name);
    if (auto& preview_url = in_json["preview_url"]; !preview_url.is_null()) {
        result.preview_url = preview_url;
    }
    return result;
}

} // namespace api

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

shard::expected<api::token_result, error_code> get_access_token(http::client& client) {
    auto request = http::request_builder()
                       .with_url("https://accounts.spotify.com/api/token")
                       .with_method(http::request::method_post)
                       .with_header("Content-Type", "application/x-www-form-urlencoded")
                       .with_post_field("grant_type", "client_credentials")
                       .with_post_field("client_id", "")
                       .with_post_field("client_secret", "")
                       .build();
    return client.send_request(std::move(request))
        .get()
        .transform_error(to_error_code)
        .and_then(to_string)
        .and_then(to_json)
        .transform(api::parse_token_result);
}

shard::expected<api::artist_result, error_code>
get_artist(http::client& client, std::string access_token, std::string id) {
    auto request = http::request_builder()
                       .with_url("https://api.spotify.com/v1/artists/" + std::move(id))
                       .with_method(http::request::method_get)
                       .with_header("Authorization", "Bearer " + std::move(access_token))
                       .build();
    return client.send_request(std::move(request))
        .get()
        .transform_error(to_error_code)
        .and_then(to_string)
        .and_then(to_json)
        .transform(api::parse_artist_result);
}

shard::expected<api::album_result, error_code>
get_album(http::client& client, std::string access_token, std::string id) {
    auto request = http::request_builder()
                       .with_url("https://api.spotify.com/v1/albums/" + std::move(id))
                       .with_method(http::request::method_get)
                       .with_header("Authorization", "Bearer " + std::move(access_token))
                       .build();
    return client.send_request(std::move(request))
        .get()
        .transform_error(to_error_code)
        .and_then(to_string)
        .and_then(to_json)
        .transform(api::parse_album_result);
}

shard::expected<api::track_result, error_code>
get_track(http::client& client, std::string access_token, std::string id) {
    auto request = http::request_builder()
                       .with_url("https://api.spotify.com/v1/tracks/" + std::move(id))
                       .with_method(http::request::method_get)
                       .with_header("Authorization", "Bearer " + std::move(access_token))
                       .build();
    return client.send_request(std::move(request))
        .get()
        .transform_error(to_error_code)
        .and_then(to_string)
        .and_then(to_json)
        .transform(api::parse_track_result);
}

shard::expected<std::string, error_code> load_access_token(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        return shard::unexpected(error_code::token_load_error);
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        return shard::unexpected(error_code::token_load_error);
    }

    std::time_t expires_at;
    file >> expires_at;

    std::string token;
    file >> token;

    if (std::time(nullptr) > expires_at) {
        return shard::unexpected(error_code::token_expired);
    }

    return token;
}

shard::expected<void, error_code> store_access_token(const std::filesystem::path& path,
                                                     const api::token_result& result) {
    std::ofstream file(path);
    if (!file.is_open()) {
        return shard::unexpected(error_code::token_store_error);
    }

    auto now = std::time(nullptr);
    file << (now + result.expires_in) << '\n';
    file << result.access_token << '\n';
    file << std::flush;

    if (file.fail()) {
        return shard::unexpected(error_code::token_store_error);
    }

    return {};
}

shard::expected<std::string, error_code> get_and_store_access_token(http::client& client,
                                                                    const std::filesystem::path& path) {
    auto result = get_access_token(client);
    if (result.has_value()) {
        store_access_token(path, *result);
        return result->access_token;
    }
    return shard::unexpected(result.error());
}

int main(int /* argc */, char* /* argv */[]) {
    http::client client;

    auto path = std::filesystem::current_path() / "token.txt";
    auto access_token =
        load_access_token(path).or_else([&](error_code) { return get_and_store_access_token(client, path); });

    std::array artists = {
        "7Eu1txygG6nJttLHbZdQOh", // Four Tet
        "4Z8W4fKeB5YxbusRsdQVPb", // Radiohead
        "13ubrt8QOOCPljQ2FL1Kca", // A$AP Rocky
    };
    auto artist =
        access_token.and_then([&](std::string token) { return get_artist(client, std::move(token), artists[0]); });
    if (artist) {
        std::cout << "artist: " << artist->name << '\n';
    } else {
        std::cerr << "failed to get artist: " << shard::to_underlying(artist.error()) << '\n';
    }

    std::array albums = {
        "2a7NyNVcy7eJSeUzzOOF4x", // Sad Night Dynamite
        "4gaNWHu5Caj3ItkYZ5i6uh", // In Colour
    };
    auto album =
        access_token.and_then([&](std::string token) { return get_album(client, std::move(token), albums[0]); });
    if (album) {
        std::cout << "album: " << album->name << '\n';
    } else {
        std::cerr << "failed to get album: " << shard::to_underlying(album.error()) << '\n';
    }

    std::array tracks = {
        "0AQquaENerGps8BQmbPw14", // Big Iron
    };
    auto track =
        access_token.and_then([&](std::string token) { return get_track(client, std::move(token), tracks[0]); });
    if (track) {
        std::cout << "track: " << track->name << '\n';
    } else {
        std::cerr << "failed to get track: " << shard::to_underlying(album.error()) << '\n';
    }

    return 0;
}
