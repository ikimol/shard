// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#include "shard/log/sink.hpp"

#include "shard/log/logger.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>

namespace shard::log {

sink::sink(std::string name, sink_writer writer, void* userdata, sink_cleanup cleanup)
: m_name(std::move(name))
, m_writer(writer)
, m_userdata(userdata)
, m_cleanup(cleanup) {}

sink::~sink() {
    if (m_cleanup) {
        m_cleanup(m_userdata);
    }
}

void sink::write(const logger& logger, const entry& entry) const {
    m_writer(logger.name(), entry, m_userdata);
}

namespace {

void console_sink_writer(const std::string& logger_name, const entry& e, void*) {
    auto& stream = e.level >= level::warn ? std::cerr : std::cout;
    auto level_char = enum_traits<level::type>::chars[e.level];
    stream << '[' << std::put_time(std::localtime(&e.timestamp), "%H:%M:%S") << "]";
    stream << "[" << level_char << "][" << std::setw(8) << std::setfill(' ') << std::right << logger_name << "] ";
    stream << e.message << '\n';
}

struct file_sink_context {
    std::ofstream stream;
};

void file_sink_writer(const std::string& logger_name, const entry& e, void* userdata) {
    auto& context = *static_cast<file_sink_context*>(userdata);
    context.stream << std::put_time(std::localtime(&e.timestamp), "%FT%T") << ',';
    context.stream << enum_traits<level::type>::names[e.level] << ',';
    context.stream << logger_name << ',';
    context.stream << e.message << '\n';
}

void file_sink_cleanup(void* userdata) {
    delete static_cast<file_sink_context*>(userdata);
}

} // namespace

sink_ptr console_sink() {
    static auto instance = std::make_shared<sink>("console", console_sink_writer);
    return instance;
}

sink_ptr make_file_sink(std::string_view path) {
    std::ofstream file(std::string(path), std::ios::app);
    if (file.is_open()) {
        auto name = std::filesystem::path(path).filename();
        auto context = new file_sink_context {std::move(file)};
        return std::make_shared<sink>(name, file_sink_writer, context, file_sink_cleanup);
    }
    return nullptr;
}

} // namespace shard::log
