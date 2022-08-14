// Copyright (c) 2022 Miklos Molnar. All rights reserved.

#ifndef SHARD_SQLITE_DATABASE_HPP
#define SHARD_SQLITE_DATABASE_HPP

#include <shard/enums/flags.hpp>

#include <sqlite3.h>

#include <cassert>
#include <memory>
#include <string>
#include <utility>

#include <iostream>

namespace shard::sqlite {

class database {
public:
    enum open_mode_t : unsigned int {
        open_read_only = SQLITE_OPEN_READONLY,
        open_read_write = SQLITE_OPEN_READWRITE,
        open_create = SQLITE_OPEN_CREATE,
    };
    SHARD_DECLARE_FLAGS(open_mode, open_mode_t)

public:
    /// Open / create a database on disk
    database(std::string filename, open_mode mode) : m_filename(std::move(filename)) {
        sqlite3* db;
        auto r = sqlite3_open_v2(m_filename.c_str(), &db, (int)(mode.value()), nullptr);
        assert(r == SQLITE_OK);
        m_handle.reset(db);
    }

    /// Execute a query without any results
    int execute(const std::string& sql) {
        sqlite3_exec(m_handle.get(), sql.c_str(), nullptr, nullptr, nullptr);
        // TODO: Handle possible error
        return sqlite3_changes(m_handle.get());
    }

    /// Check if a table with the given name exists
    bool does_table_exist(const std::string& name);

    /// Get the underlying handle
    sqlite3* handle() const { return m_handle.get(); }

private:
    struct deleter {
        void operator()(sqlite3* db) { sqlite3_close(db); }
    };

private:
    std::string m_filename;
    std::unique_ptr<sqlite3, deleter> m_handle;
};

SHARD_DECLARE_FLAG_OPERATORS(database::open_mode);

} // namespace shard::sqlite

#endif // SHARD_SQLITE_DATABASE_HPP
