// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_SQLITE_TRANSACTION_HPP
#define SHARD_SQLITE_TRANSACTION_HPP

#include "shard/sqlite/database.hpp"

namespace shard::sqlite {

class transaction {
public:
    /// Begin a new transaction
    explicit transaction(database& db) : m_db(db) { m_db.execute("BEGIN"); }

    /// Roll back the transaction if it was not committed
    ~transaction() {
        if (!m_committed) {
            try {
                m_db.execute("ROLLBACK");
            } catch (...) {}
        }
    }

    /// Commit the changes made in this transaction
    void commit() {
        if (!m_committed) {
            m_db.execute("COMMIT");
            m_committed = true;
        } else {
            // TODO: Handle possible error
        }
    }

private:
    database& m_db;
    bool m_committed = false;
};

} // namespace shard::sqlite

#endif // SHARD_SQLITE_TRANSACTION_HPP
