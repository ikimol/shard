// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/sqlite.hpp>

#include <iostream>

namespace sqlite = shard::sqlite;

static std::string divider(40, '-');

struct user {
    int id = -1;
    std::string username;
    std::string password;
    int reg_date = -1;
};

std::ostream& operator<<(std::ostream& os, const user& u) {
    return os << "user(" << u.id << "): " << u.username;
}

std::optional<user> fetch_user(sqlite::statement& stmt) {
    user u;
    if (stmt.fetch(u.id, u.username, u.password, u.reg_date)) {
        return u;
    }
    return std::nullopt;
}

int main(int /* argc */, char* /* argv */[]) {
    sqlite::database db("test.db", sqlite::database::open_read_write);
    std::cout << "has 'users' table: " << db.does_table_exist("users") << '\n';
    std::cout << "has 'comments' table: " << db.does_table_exist("comments") << '\n';
    std::cout << divider << '\n';

    sqlite::statement stmt(db, "SELECT * FROM users WHERE id>?");
    sqlite::bind(stmt, 1);
    std::cout << "columns: " << stmt.column_count() << '\n';
    std::cout << divider << '\n';

    // process manually
    while (stmt.step()) {
        int id = stmt.get_column(0);
        std::string_view username = stmt.get_column(1);
        std::cout << "user(" << id << "): " << username << '\n';
    }
    assert(stmt.is_done());
    std::cout << divider << '\n';

    stmt.reset();

    // process using lambda
    stmt.apply([](int id, std::string username, std::string, int) /* NOLINT */ {
        // print a record
        std::cout << "user(" << id << "): " << username << '\n';
    });
    assert(stmt.is_done());
    std::cout << divider << '\n';

    stmt.reset();

    // process using custom class
    while (auto u = fetch_user(stmt)) {
        std::cout << *u << '\n';
    }
    assert(stmt.is_done());
    std::cout << divider << '\n';

    return 0;
}
