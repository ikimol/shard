// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include "shard/sqlite/column.hpp"
#include "shard/sqlite/database.hpp"
#include "shard/sqlite/error.hpp"

#include <shard/memory/data.hpp>
#include <shard/meta/type_traits.hpp>
#include <shard/utility/preprocessor.hpp>

#include <cstdint>
#include <memory>
#include <optional>
#include <utility>

namespace shard::sqlite {

class statement {
public:
    statement(database& db, std::string sql)
    : m_db(db)
    , m_sql(std::move(sql))
    , m_statement(prepare()) {
        m_column_count = sqlite3_column_count(m_statement.get());
    }

    // value binding

    /// Bind a boolean value as an integer
    void bind(int index, bool value) { sqlite3_bind_int(m_statement.get(), index, value ? 1 : 0); }

    /// Bind an integer value
    void bind(int index, int value) { sqlite3_bind_int(m_statement.get(), index, value); }

    /// Bind a 64 bit integer value
    void bind(int index, std::int64_t value) { sqlite3_bind_int64(m_statement.get(), index, value); }

    /// Bind a floating point value
    void bind(int index, double value) { sqlite3_bind_double(m_statement.get(), index, value); }

    /// Bind a raw string array
    void bind(int index, const char* value) {
        sqlite3_bind_text(m_statement.get(), index, value, (int)(std::strlen(value)), SQLITE_TRANSIENT);
    }

    /// Bind a string value
    void bind(int index, const std::string& value) {
        sqlite3_bind_text(m_statement.get(), index, value.c_str(), (int)(value.length()), SQLITE_TRANSIENT);
    }

    /// Bind raw data
    void bind(int index, const void* value, std::size_t size) {
        sqlite3_bind_blob(m_statement.get(), index, value, (int)(size), SQLITE_TRANSIENT);
    }

    /// Bind raw data
    void bind(int index, const memory::data& data) {
        sqlite3_bind_blob(m_statement.get(), index, data.bytes, (int)(data.size), SQLITE_TRANSIENT);
    }

    /// Bind an optional value as the value or null
    template <typename T>
    void bind(int index, const std::optional<T>& value) {
        value ? bind(index, *value) : bind(index);
    }

    /// Bind a null value
    void bind(int index) { sqlite3_bind_null(m_statement.get(), index); }

    /// Bind a value using its name in the statement
    template <typename T>
    void bind(const std::string& name, T&& value) {
        auto index = sqlite3_bind_parameter_index(m_statement.get(), name.c_str());
        bind(index, SHARD_FWD(value));
    }

    /// Step the query once
    ///
    /// \return true if there's more rows to be processed, false otherwise
    bool step() {
        auto r = do_step();
        if ((r != SQLITE_ROW) && (r != SQLITE_DONE)) {
            throw database_error(m_db.handle());
        }
        return m_row_available;
    }

    /// Execute the query with no expected result
    ///
    /// \return The number of updated rows
    int execute() {
        auto r = do_step();
        if (r != SQLITE_DONE) {
            if (r == SQLITE_ROW) {
                throw generic_error(r, "unexpected result");
            } else {
                throw database_error(m_db.handle());
            }
        }
        return sqlite3_changes(m_db.handle());
    }

    /// Apply a functor to each row of the result
    template <typename F>
    void apply(F&& fn) {
        using traits = meta::functor_traits<F>;
        using args_type = typename traits::args_type;

        auto read = [this](auto&&... args) {
            int i = -1;
            (read_value(m_statement.get(), ++i, SHARD_FWD(args)), ...);
        };

        args_type arguments;

        do {
            auto r = do_step();
            if (r == SQLITE_ROW) {
                std::apply(read, arguments);
                std::apply(fn, std::move(arguments));
            } else if (r == SQLITE_DONE) {
                break;
            } else {
                throw database_error(m_db.handle());
            }
        } while (m_row_available);
    }

    /// Fetch a single row into the given variables
    template <typename... Args>
    bool fetch(Args&... args) {
        auto r = do_step();
        if (r == SQLITE_ROW) {
            int i = -1;
            (read_value(m_statement.get(), ++i, SHARD_FWD(args)), ...);
            return true;
        } else if (r == SQLITE_DONE) {
            return false;
        } else {
            throw database_error(m_db.handle());
        }
    }

    /// Check if there's another row to be processed
    bool has_next() const { return m_row_available; }

    /// Check if all rows have been processed
    bool is_done() const { return m_done; }

    /// Get the number of columns
    std::size_t column_count() const { return m_column_count; }

    /// Get the name of a column
    std::string column_name(int index) const { return sqlite3_column_name(m_statement.get(), index); }

    /// Get a single column value
    column get_column(int index) const { return {m_statement, index}; }

    /// Reset the statement, making it ready for another execution
    void reset() {
        m_row_available = false;
        m_done = false;
        auto r = sqlite3_reset(m_statement.get());
        if (r != SQLITE_OK) {
            throw database_error(m_db.handle());
        }
    }

private:
    std::shared_ptr<sqlite3_stmt> prepare() {
        sqlite3_stmt* statement;
        sqlite3_prepare_v2(m_db.handle(), m_sql.c_str(), static_cast<int>(m_sql.size()), &statement, nullptr);
        // TODO: Handle possible error
        auto deleter = [](sqlite3_stmt* statement) {
            sqlite3_finalize(statement);
        };
        return {statement, deleter};
    }

    int do_step() {
        if (!m_done) {
            auto r = sqlite3_step(m_statement.get());
            if (r == SQLITE_ROW) { /* a row is ready */
                m_row_available = true;
            } else {
                m_row_available = false;
                m_done = (r == SQLITE_DONE);
            }
            return r;
        } else {
            // statement needs to be reset
            return SQLITE_MISUSE;
        }
    }

    // value reading

    static void read_value(sqlite3_stmt* statement, int column_index, bool& out) {
        out = sqlite3_column_int(statement, column_index);
    }

    static void read_value(sqlite3_stmt* statement, int column_index, int& out) {
        out = sqlite3_column_int(statement, column_index);
    }

    static void read_value(sqlite3_stmt* statement, int column_index, std::int64_t& out) {
        out = sqlite3_column_int64(statement, column_index);
    }

    static void read_value(sqlite3_stmt* statement, int column_index, double& out) {
        out = sqlite3_column_double(statement, column_index);
    }

    static void read_value(sqlite3_stmt* statement, int column_index, std::string& out) {
        auto str = reinterpret_cast<const char*>(sqlite3_column_text(statement, column_index));
        if (!str) {
            return;
        }
        out = str;
    }

    static void read_value(sqlite3_stmt* statement, int column_index, memory::data& out) {
        out.bytes = reinterpret_cast<std::byte*>(const_cast<void*>(sqlite3_column_blob(statement, column_index)));
        out.size = sqlite3_column_bytes(statement, column_index);
    }

    template <typename T>
    void read_value(sqlite3_stmt* statement, int column_index, std::optional<T>& out) {
        if (sqlite3_column_type(statement, column_index) == SQLITE_NULL) {
            out = std::nullopt;
        } else {
            T value;
            read_value(statement, column_index, value);
            out = value;
        }
    }

private:
    database& m_db;
    std::string m_sql;
    std::shared_ptr<sqlite3_stmt> m_statement;

    bool m_row_available = false;
    bool m_done = false;

    std::size_t m_column_count = 0;
};

// helper functions

/// Bind the values to the statement
template <typename... Args>
void bind(statement& stmt, Args&&... args) {
    int i = 0;
    (stmt.bind(++i, SHARD_FWD(args)), ...);
}

template <typename... Args>
void bind(statement& stmt, const std::tuple<Args...>& values) {
    bind(stmt, values, std::index_sequence_for<Args...>());
}

template <typename... Args, std::size_t... Is>
void bind(statement& stmt, const std::tuple<Args...>& values, std::index_sequence<Is...>) {
    bind(stmt, std::get<Is>(values)...);
}

// implementation

bool database::contains_table(const std::string& name) {
    statement stmt(*this, "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name=?");
    stmt.bind(1, name);
    stmt.step();
    return stmt.get_column(0).as_int() == 1;
}

} // namespace shard::sqlite
