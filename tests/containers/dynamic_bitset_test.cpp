// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include <shard/dynamic_bitset.hpp>

#include <doctest.h>

TEST_CASE("containers") {
    SUBCASE("dynamic_bitset") {
        shard::dynamic_bitset bits(8);

        SUBCASE("size and capacity") {
            REQUIRE_FALSE(bits.empty());

            REQUIRE(bits.num_blocks() == 1);
            REQUIRE(bits.size() == 8);
            REQUIRE(bits.capacity() == 32);

            bits.reserve(64);
            REQUIRE(bits.num_blocks() == 1);
            REQUIRE(bits.size() == 8);
            REQUIRE(bits.capacity() == 64);

            bits.shrink_to_fit();
            REQUIRE(bits.num_blocks() == 1);
            REQUIRE(bits.size() == 8);
            REQUIRE(bits.capacity() == 32);

            bits.resize(64);
            REQUIRE(bits.num_blocks() == 2);
            REQUIRE(bits.size() == 64);
            REQUIRE(bits.capacity() == 64);
        }

        SUBCASE("resize") {
            bits.resize(40);
            REQUIRE(bits.num_blocks() == 2);

            bits.set(0).set(31).set(32).set(39);
            REQUIRE(bits.count() == 4);

            bits.resize(64, true);
            REQUIRE(bits.count() == 28);
        }

        SUBCASE("set and test") {
            bits.set(0);
            REQUIRE(bits.test(0));

            bits.reset(0);
            REQUIRE_FALSE(bits.test(0));

            bits.flip(0);
            REQUIRE(bits.test(0));

            bits.set(0).set(1);
            REQUIRE(bits.test_set(0, false));
            REQUIRE(bits.test_set(1, false));
            REQUIRE(bits.none());
        }

        SUBCASE("reference") {
            auto bit = bits[0];
            REQUIRE_FALSE(bits.test(0));

            bit.set();
            REQUIRE(bit);
            REQUIRE(bits.test(0));

            bit.reset();
            REQUIRE_FALSE(bit);
            REQUIRE_FALSE(bits.test(0));

            bit.flip();
            REQUIRE(bit);
            REQUIRE(bits.test(0));

            bit = false;
            REQUIRE_FALSE(bit);
            REQUIRE_FALSE(bits.test(0));
        }

        SUBCASE("count") {
            REQUIRE(bits.count() == 0);

            bits.set(0);
            bits.set(1);
            REQUIRE(bits.count() == 2);

            bits.reset(0);
            REQUIRE(bits.count() == 1);

            bits.resize(64);
            REQUIRE(bits.count() == 1);

            for (auto i = 0ul; i < bits.size(); ++i) {
                bits.set(i, i % 2 == 0);
            }
            REQUIRE(bits.count() == 32);
        }

        SUBCASE("operators") {
            SUBCASE("bitwise and") {
                shard::dynamic_bitset mask(8, 0b1111'1111);

                auto bits_and = bits & mask; // 0000 0000
                REQUIRE(bits_and.none());

                bits.set(0).set(2); // 0000 0101
                bits_and = bits & mask;
                REQUIRE(bits_and.count() == 2);
                REQUIRE(bits_and.test(0));
                REQUIRE(bits_and.test(2));
            }

            SUBCASE("bitwise or") {
                shard::dynamic_bitset mask(8, 0b1010'1010);
                bits = shard::dynamic_bitset(8, 0b0101'0101);

                auto bits_or = bits | mask; // 1111 1111
                REQUIRE(bits_or.all());

                bits.reset();
                bits_or = bits | mask; // 1010 1010
                REQUIRE(bits_or.count() == 4);
                REQUIRE(bits_or.test(1));
                REQUIRE(bits_or.test(3));
                REQUIRE(bits_or.test(5));
                REQUIRE(bits_or.test(7));
            }

            SUBCASE("bitwise xor") {
                shard::dynamic_bitset mask(8, 0b1100'0011);
                bits = shard::dynamic_bitset(8, 0b0011'1100);

                auto bits_xor = bits ^ mask; // 1111 1111
                REQUIRE(bits_xor.all());

                mask.set();
                bits_xor = mask ^ bits; // 1100 0011
                REQUIRE(bits_xor.count() == 4);
                REQUIRE(bits_xor.test(0));
                REQUIRE(bits_xor.test(1));
                REQUIRE(bits_xor.test(6));
                REQUIRE(bits_xor.test(7));
            }

            SUBCASE("difference") {
                shard::dynamic_bitset mask(8, 0b0011'1100);
                bits.set(); // 1111 1111

                auto bits_sub = bits - mask; // 1100 0011
                REQUIRE(bits_sub.count() == 4);
                REQUIRE(bits_sub.test(0));
                REQUIRE(bits_sub.test(1));
                REQUIRE(bits_sub.test(6));
                REQUIRE(bits_sub.test(7));

                mask.set(6).set(7);
                bits_sub = bits - mask;
                REQUIRE(bits_sub.count() == 2);
                REQUIRE(bits_sub.test(0));
                REQUIRE(bits_sub.test(1));
            }

            SUBCASE("negation") {
                REQUIRE(bits.none());
                bits = ~bits;
                REQUIRE(bits.all());
            }
        }

        SUBCASE("iteration") {
            for (auto i = 0ul; i < bits.size(); ++i) {
                bits.set(i, i % 2 == 0);
            }
            REQUIRE(bits.count() == 4);

            std::vector<std::size_t> indices;
            for (auto i = bits.find_first(); i != shard::dynamic_bitset<>::npos; i = bits.find_next(i)) {
                indices.push_back(i);
            }
            REQUIRE(indices.size() == 4);
            REQUIRE(indices[0] == 0);
            REQUIRE(indices[1] == 2);
            REQUIRE(indices[2] == 4);
            REQUIRE(indices[3] == 6);
        }

        SUBCASE("conversion") {
            REQUIRE(bits.to_string() == "00000000");

            bits.set();
            REQUIRE(bits.to_string() == "11111111");

            bits.pop_back();
            bits.push_back(false);
            REQUIRE(bits.to_string() == "01111111");

            bits.reset(5).reset(3).reset(1);
            REQUIRE(bits.to_string('-', '+') == "-+-+-+-+");
        }

        SUBCASE("block size") {
            static_assert(shard::dynamic_bitset<std::uint64_t>::bits_per_block == 64);

            shard::dynamic_bitset<std::uint64_t> bits_64(64);
            REQUIRE(bits_64.num_blocks() == 1);

            bits_64.push_back(false);
            REQUIRE(bits_64.num_blocks() == 2);
        }
    }
}
