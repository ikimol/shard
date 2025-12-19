// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/io/open_mode.hpp"

#include <shard/memory/dynamic_data.hpp>

#include <cassert>
#include <string>

namespace shard {
namespace io {

/// Represents a file on disk
class file {
public:
    /// Default constructor
    file() = default;

    /// Constructor to open a file
    explicit file(const std::string& path, open_mode_t mode = open_mode::read) { open(path, mode); }

    /// Destructor
    ~file() { close(); }

    // disable copying

    file(const file&) = delete;
    file& operator=(const file&) = delete;

    /// Move constructor
    file(file&& other) noexcept
    : m_handle(std::exchange(other.m_handle, nullptr)) { }

    /// Move assignment operator
    file& operator=(file&& other) noexcept {
        if (this != &other) {
            close();
            m_handle = std::exchange(other.m_handle, nullptr);
        }
        return *this;
    }

    /// Open a file
    bool open(const std::string& path, open_mode_t mode = open_mode::read) {
        close();
        m_handle = std::fopen(path.c_str(), to_string(mode).c_str());
        return is_open();
    }

    /// Close the file
    void close() noexcept {
        if (m_handle) {
            std::fclose(m_handle);
            m_handle = nullptr;
        }
    }

    /// Check if the file is open
    bool is_open() const noexcept { return m_handle != nullptr; }

    /// Check if the stream has any errors
    bool has_error() const noexcept { return std::ferror(m_handle) != 0; }

    /// Get the size of the file in bytes
    std::size_t size() const {
        assert(is_open());

        auto current = std::ftell(m_handle);
        std::fseek(m_handle, 0, SEEK_END);
        auto size = std::ftell(m_handle);
        std::fseek(m_handle, current, SEEK_SET);

        return static_cast<std::size_t>(size);
    }

    // contents

    /// Get the contents of the file as bytes
    dynamic_data contents() const {
        assert(is_open());

        auto file_size = size();
        dynamic_data content(file_size);

        std::rewind(m_handle);
        auto read_size = std::fread(content.bytes(), 1, file_size, m_handle);
        assert(read_size == file_size);

        return content;
    }

    // reading

    /// Read a number of bytes
    std::size_t read(void* out, std::size_t size, std::size_t count) const {
        assert(is_open());
        return std::fread(out, size, count, m_handle);
    }

    /// Read an array of objects
    template <typename T>
    std::size_t read(T* out, std::size_t count) const {
        return read(out, sizeof(T), count);
    }

    // writing

    /// Write a number of bytes
    std::size_t write(const void* ptr, std::size_t size, std::size_t count) {
        assert(is_open());
        return std::fwrite(ptr, size, count, m_handle);
    }

    /// Write an array of objects
    template <typename T>
    std::size_t write(const T* array, std::size_t count) {
        return write(array, sizeof(T), count);
    }

    /// Flush any unwritten data
    void flush() { std::fflush(m_handle); }

    /// Bool conversion operator
    explicit operator bool() const noexcept { return is_open(); }

private:
    std::FILE* m_handle = nullptr;
};

} // namespace io

// bring symbols into parent namespace

using io::file;

} // namespace shard
