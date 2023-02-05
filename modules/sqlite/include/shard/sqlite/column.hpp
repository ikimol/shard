// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_SQLITE_COLUMN_HPP
#define SHARD_SQLITE_COLUMN_HPP

#include <sqlite3.h>

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>

namespace shard::sqlite {

class column {
    friend class statement;

public:
    enum data_type : int {
        integer = SQLITE_INTEGER,
        floating = SQLITE_FLOAT,
        string = SQLITE_TEXT,
        blob = SQLITE_BLOB,
        null = SQLITE_NULL,
    };

public:
    // type checks

    data_type type() const { return static_cast<data_type>(sqlite3_column_type(m_statement.get(), m_index)); }

    bool is_int() const { return type() == integer; }

    bool is_double() const { return type() == floating; }

    bool is_string() const { return type() == string; }

    bool is_blob() const { return type() == blob; }

    bool is_null() const { return type() == null; }

    // data access

    int as_int() const {
        assert(is_int());
        return sqlite3_column_int(m_statement.get(), m_index);
    }

    std::int64_t as_int64() const {
        assert(is_int());
        return sqlite3_column_int64(m_statement.get(), m_index);
    }

    double as_double() const {
        assert(is_double());
        return sqlite3_column_double(m_statement.get(), m_index);
    }

    const char* as_raw_string() const {
        assert(is_string());
        return reinterpret_cast<const char*>(sqlite3_column_text(m_statement.get(), m_index));
    }

    std::string as_string() const {
        assert(is_string());
        auto data = static_cast<const char*>(sqlite3_column_blob(m_statement.get(), m_index));
        std::size_t length = sqlite3_column_bytes(m_statement.get(), m_index);
        return std::string {data, length};
    }

    std::string_view as_string_view() const {
        assert(is_string());
        auto data = static_cast<const char*>(sqlite3_column_blob(m_statement.get(), m_index));
        std::size_t length = sqlite3_column_bytes(m_statement.get(), m_index);
        return std::string_view {data, length};
    }

    const void* as_blob() const {
        assert(is_blob());
        return sqlite3_column_blob(m_statement.get(), m_index);
    }

    std::size_t size() const { return sqlite3_column_bytes(m_statement.get(), m_index); }

    // implicit cast operators

    operator int() const /* NOLINT */ { return as_int(); }

    operator std::int64_t() const /* NOLINT */ { return as_int64(); }

    operator double() const /* NOLINT */ { return as_double(); }

    operator const char*() const /* NOLINT */ { return as_raw_string(); }

    operator std::string() const /* NOLINT */ { return as_string(); }

    operator std::string_view() const /* NOLINT */ { return as_string_view(); }

    operator const void*() const /* NOLINT */ { return as_blob(); }

private:
    column(std::shared_ptr<sqlite3_stmt> statement, int index)
    : m_statement(std::move(statement))
    , m_index(index) {}

private:
    std::shared_ptr<sqlite3_stmt> m_statement;
    int m_index;
};

} // namespace shard::sqlite

#endif // SHARD_SQLITE_COLUMN_HPP
