// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_SQLITE_ERROR_HPP
#define SHARD_SQLITE_ERROR_HPP

#include <shard/utility/error.hpp>

#include <sqlite3.h>

namespace shard::sqlite {

class database_error : public shard::error {
public:
    explicit database_error(sqlite3* db) : shard::error(sqlite3_errcode(db), sqlite3_errmsg(db)) {}
};

class generic_error : public shard::error {
public:
    // user base class constructor
    using shard::error::basic_error;
};

} // namespace shard::sqlite

#endif // SHARD_SQLITE_ERROR_HPP
