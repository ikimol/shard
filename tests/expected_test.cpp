// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include "helpers/counter.hpp"
#include "helpers/no_default.hpp"
#include "helpers/widget.hpp"

#include <shard/expected.hpp>

#include <doctest.h>

struct error {
    int code;
};

static bool operator==(const error& lhs, const error& rhs) {
    return lhs.code == rhs.code;
}

static shard::expected<int, error> succeed(int value = 0) {
    return value;
}

static shard::expected<int, error> fail(int error_code = -1) {
    return shard::unexpected(error {error_code});
}

TEST_CASE("expected") {
    SUBCASE("constructors") {
        SUBCASE("default") {
            shard::expected<test::widget, error> ex;
            REQUIRE(ex.has_value());
            REQUIRE(ex.value().a == 0);
        }

        SUBCASE("copy") {
            test::counter::reset();
            shard::expected<test::counter, error> ex;
            REQUIRE(ex.has_value());
            REQUIRE(test::counter::default_constructor == 1);

            auto copy = ex; /* NOLINT */
            REQUIRE(copy.has_value());
            REQUIRE(test::counter::copy_constructor == 1);

            REQUIRE(ex.has_value());
        }

        SUBCASE("move") {
            test::counter::reset();
            shard::expected<test::counter, error> ex;
            REQUIRE(ex.has_value());
            REQUIRE(test::counter::default_constructor == 1);

            auto moved_to = std::move(ex);
            REQUIRE(test::counter::move_constructor == 1);
            REQUIRE(test::counter::instances == 2);
        }

        SUBCASE("unexpected") {
            auto ex = fail(-1);
            REQUIRE_FALSE(ex.has_value());
            REQUIRE(ex.error().code == -1);
        }

        SUBCASE("value") {
            SUBCASE("primitive") {
                auto ex = succeed(42);
                REQUIRE(ex.has_value());
                REQUIRE(ex.value() == 42);
            }

            SUBCASE("no default") {
                shard::expected<test::no_default, error> ex = test::no_default(4, 5);
                REQUIRE(ex.has_value());
                REQUIRE(ex.value().a == 4);
                REQUIRE(ex.value().b == 5);
            }
        }
    }

    SUBCASE("assignment") {
        SUBCASE("copy") {
            test::counter::reset();
            shard::expected<test::counter, error> ex;
            REQUIRE(test::counter::default_constructor == 1);
            shard::expected<test::counter, error> tmp;
            REQUIRE(test::counter::default_constructor == 2);
            ex = tmp;
            REQUIRE(test::counter::copy_assignment_op == 1);
        }

        SUBCASE("move") {
            test::counter::reset();
            shard::expected<test::counter, error> ex;
            REQUIRE(test::counter::default_constructor == 1);
            ex = shard::expected<test::counter, error>();
            REQUIRE(test::counter::default_constructor == 2);
            REQUIRE(test::counter::move_assignment_op == 1);
        }

        SUBCASE("unexpected") {
            shard::expected<int, error> ex;
            REQUIRE(ex.has_value());
            ex = shard::unexpected(error {-2});
            REQUIRE_FALSE(ex.has_value());
            REQUIRE(ex.error().code == -2);
        }

        SUBCASE("value") {
            SUBCASE("primitive") {
                shard::expected<int, error> ex;
                REQUIRE(ex.has_value());
                ex = 42;
                REQUIRE(ex.has_value());
                REQUIRE(ex.value() == 42);
            }

            SUBCASE("no default") {
                shard::expected<test::no_default, error> ex = test::no_default(0, 1);
                REQUIRE(ex.has_value());
                REQUIRE(ex.value().a == 0);
                REQUIRE(ex.value().b == 1);

                ex = test::no_default(2, 3);
                REQUIRE(ex.has_value());
                REQUIRE(ex.value().a == 2);
                REQUIRE(ex.value().b == 3);
            }
        }
    }

    SUBCASE("operator bool") {
        auto ex1 = succeed();
        REQUIRE(bool(ex1));

        auto ex2 = fail();
        REQUIRE_FALSE(ex2);
    }

    SUBCASE("value access") {
        shard::expected<test::widget, error> ex;
        REQUIRE(ex->a == 0);
        REQUIRE((*ex).a == 0);

        SUBCASE("bad access") {
            shard::expected<int, error> ue = shard::unexpected(error {});
            REQUIRE_THROWS_AS(ue.value(), shard::bad_expected_access<error>);
        }
    }

    SUBCASE("equality") {
        shard::expected<int, error> ex1;
        shard::expected<int, error> ex2;
        REQUIRE(ex1 == ex2);

        ex1 = 21;
        REQUIRE(ex1 != ex2);

        REQUIRE(ex1 != shard::unexpected(error {}));
    }

    SUBCASE("monadic") {
        SUBCASE("and_then") {
            auto called = false;
            auto handler = [&](int i) -> shard::expected<int, error> {
                called = true;
                return i;
            };

            succeed().and_then(handler);
            REQUIRE(called);

            called = false;
            fail().and_then(handler);
            REQUIRE_FALSE(called);
        }

        SUBCASE("or_else") {
            auto called = false;
            auto handler = [&](const error& e) -> shard::expected<int, error> {
                called = true;
                return shard::unexpected(e);
            };

            fail().or_else(handler);
            REQUIRE(called);

            called = false;
            succeed().or_else(handler);
            REQUIRE_FALSE(called);
        }

        SUBCASE("transform") {
            auto called = false;
            auto handler = [&](int i) -> int {
                called = true;
                return i;
            };

            auto result = succeed(42).transform(handler);
            REQUIRE(called);
            REQUIRE(*result == 42);

            called = false;
            fail().transform(handler);
            REQUIRE_FALSE(called);
        }
    }

    SUBCASE("void") {
        shard::expected<void, error> ex;
        REQUIRE(ex.has_value());
        REQUIRE(std::is_void_v<decltype(ex.value())>);

        SUBCASE("equality") {
            shard::expected<void, error> ex1;
            shard::expected<void, error> ex2;
            REQUIRE(ex1 == ex2);

            REQUIRE(ex1 != shard::unexpected(error {}));
        }

        SUBCASE("monadic") {
            auto called = false;
            ex.and_then([&]() -> shard::expected<void, error> {
                called = true;
                return {};
            });
        }
    }
}
