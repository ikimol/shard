// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#include <shard/io/file.hpp>

#include <doctest.h>

#include <filesystem>

TEST_CASE("io") {
    const std::string test_content = "foo bar";
    const std::string test_file = "test_file.txt";
    std::filesystem::remove(test_file);

    SUBCASE("read and write") {
        {
            shard::io::file f(test_file, shard::io::open_mode::write);
            REQUIRE(f.is_open());
            auto written = f.write(test_content.data(), test_content.size());
            CHECK(written == test_content.size());
        }

        {
            shard::io::file f(test_file, shard::io::open_mode::read);
            REQUIRE(f.is_open());
            auto contents = f.contents();
            auto str_contents = std::string_view(reinterpret_cast<char*>(contents.bytes()), contents.size());
            CHECK(str_contents == test_content);
            CHECK(f.size() == test_content.size());
        }
    }

    SUBCASE("binary") {
        const std::string binary_file = "test_binary.bin";
        std::filesystem::remove(binary_file);

        std::vector<std::uint8_t> binary_data = {0xde, 0xad, 0xbe, 0xef, 0x00, 0xff};

        {
            shard::data d(binary_data.data(), binary_data.size());
            shard::io::file f(binary_file, shard::io::open_mode::write | shard::io::open_mode::binary);
            REQUIRE(f.is_open());
            auto written = f.write(d.bytes(), d.size());
            CHECK(written == binary_data.size());
        }

        {
            shard::io::file f(binary_file, shard::io::open_mode::read | shard::io::open_mode::binary);
            REQUIRE(f.is_open());
            auto read_data = f.contents();
            CHECK(read_data.size() == binary_data.size());
            CHECK(std::memcmp(read_data.bytes(), binary_data.data(), binary_data.size()) == 0);
        }
    }

    SUBCASE("append") {
        {
            shard::io::file f(test_file, shard::io::open_mode::write);
            f.write(test_content.data(), test_content.size());
        }

        const std::string append_content = " baz";
        {
            shard::io::file f(test_file, shard::io::open_mode::append);
            REQUIRE(f.is_open());
            f.write(append_content.data(), append_content.size());
        }

        {
            shard::io::file f(test_file, shard::io::open_mode::read);
            REQUIRE(f.is_open());
            auto contents = f.contents();
            auto str_contents = std::string_view(reinterpret_cast<char*>(contents.bytes()), contents.size());
            CHECK(str_contents == test_content + append_content);
            CHECK(f.size() == test_content.size() + append_content.size());
        }
    }

    SUBCASE("exclusive") {
        errno = 0;

        {
            shard::io::file f(test_file, shard::io::open_mode::write | shard::io::open_mode::exclusive);
            REQUIRE(f.is_open());
            CHECK_FALSE(f.has_error());
            std::error_code ec(errno, std::system_category());
            CHECK(ec == std::error_code {});
        }

        {
            shard::io::file f(test_file, shard::io::open_mode::write | shard::io::open_mode::exclusive);
            REQUIRE_FALSE(f.is_open());
            std::error_code ec(errno, std::system_category());
            CHECK(ec == std::errc::file_exists);
        }
    }

    SUBCASE("move") {
        {
            shard::io::file f(test_file, shard::io::open_mode::write);
            f.write(test_content.data(), test_content.size());
        }

        shard::io::file f1(test_file, shard::io::open_mode::read);
        REQUIRE(f1.is_open());

        shard::io::file f2 = std::move(f1);
        CHECK(!f1.is_open());
        CHECK(f2.is_open());
        CHECK(f2.size() > 0);
    }
}
