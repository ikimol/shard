// Copyright (c) 2024 Miklos Molnar. All rights reserved.

#include <shard/dynamic_bitset.hpp>

#include <doctest.h>

TEST_CASE("containers.dynamic_bitset") {
    SUBCASE("constructor") {
        SUBCASE("zero bits") {
            shard::dynamic_bitset bits(0);
            REQUIRE(bits.empty());
            REQUIRE(bits.size() == 0);
            REQUIRE(bits.num_blocks() == 0);
            REQUIRE(bits.count() == 0);
            REQUIRE(bits.none());
            REQUIRE(bits.all());
        }

        SUBCASE("value larger than size") {
            // value is 64 bits, but only 32 bits allocated
            shard::dynamic_bitset bits(32, 0xFFFFFFFF'FFFFFFFF);
            REQUIRE(bits.count() == 32);
            REQUIRE(bits.all());
        }

        SUBCASE("value across multiple blocks") {
            // 4 byte blocks
            shard::dynamic_bitset bits_multi(64, 0xF0F0F0F0'0F0F0F0F);
            REQUIRE(bits_multi.count() == 32); // 4 bits per nibble * 8 nibbles

            // check pattern
            for (auto i = 0ul; i < 64; i += 4) {
                if (i < 32) {
                    // lower 32 bits: 0F0F0F0F pattern
                    REQUIRE(bits_multi.test(i) == ((i / 4) % 2 == 0));
                } else {
                    // upper 32 bits: F0F0F0F0 pattern
                    REQUIRE(bits_multi.test(i) == ((i / 4) % 2 == 1));
                }
            }
        }
    }

    SUBCASE("size and capacity") {
        shard::dynamic_bitset bits(8);

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
        shard::dynamic_bitset bits;

        bits.resize(40);
        REQUIRE(bits.num_blocks() == 2);

        bits.set(0).set(31).set(32).set(39);
        REQUIRE(bits.count() == 4);

        bits.resize(64, true);
        REQUIRE(bits.count() == 28);
    }

    SUBCASE("set and test") {
        shard::dynamic_bitset bits(2);

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
        shard::dynamic_bitset bits(8);

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
        SUBCASE("empty bitset") {
            shard::dynamic_bitset empty;
            REQUIRE(empty.count() == 0);
            REQUIRE(empty.empty());
        }

        SUBCASE("single bit in various positions") {
            shard::dynamic_bitset bits(64);

            // first bit
            bits.set(0);
            REQUIRE(bits.count() == 1);
            bits.reset(0);

            // last bit of first block
            bits.set(31);
            REQUIRE(bits.count() == 1);
            bits.reset(31);

            // first bit of second block
            bits.set(32);
            REQUIRE(bits.count() == 1);
            bits.reset(32);

            // last bit
            bits.set(63);
            REQUIRE(bits.count() == 1);
        }

        SUBCASE("count after operations") {
            shard::dynamic_bitset bits(64);
            bits.set();
            REQUIRE(bits.count() == 64);

            bits.flip();
            REQUIRE(bits.count() == 0);

            bits.push_back(true);
            REQUIRE(bits.count() == 1);
            REQUIRE(bits.size() == 65);

            bits.pop_back();
            REQUIRE(bits.count() == 0);
            REQUIRE(bits.size() == 64);
        }

        SUBCASE("multiple blocks with patterns") {
            shard::dynamic_bitset bits(128);

            // alternating blocks
            for (auto i = 0ul; i < 32; ++i) {
                bits.set(i);      // first block: all set
                bits.set(64 + i); // third block: all set
            }
            REQUIRE(bits.count() == 64);

            // set remaining bits
            bits.set();
            REQUIRE(bits.count() == 128);
        }
    }

    SUBCASE("contains") {
        shard::dynamic_bitset bits_1(8, 0b1111'0000);
        shard::dynamic_bitset bits_2(8, 0b1100'0000);
        shard::dynamic_bitset bits_3(8, 0b1111'1111);

        REQUIRE(bits_1.contains(bits_2));       // bits1 has all bits of bits2
        REQUIRE_FALSE(bits_2.contains(bits_1)); // bits2 doesn't have all bits of bits1
        REQUIRE(bits_3.contains(bits_1));       // all set contains subset
        REQUIRE(bits_3.contains(bits_2));       // all set contains subset

        // self-containment
        REQUIRE(bits_1.contains(bits_1));

        // empty containment
        shard::dynamic_bitset empty(8);
        REQUIRE(bits_1.contains(empty));
        REQUIRE(empty.contains(empty));

        // different sizes
        shard::dynamic_bitset bits_4(16, 0b0000'0000'0000'1111);
        shard::dynamic_bitset bits_5(4, 0b0001);
        REQUIRE(bits_3.contains(bits_4));
        REQUIRE(bits_4.contains(bits_5));
    }

    SUBCASE("operators") {
        SUBCASE("bitwise and") {
            shard::dynamic_bitset bits(8);
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
            shard::dynamic_bitset bits(8, 0b0101'0101);
            shard::dynamic_bitset mask(8, 0b1010'1010);

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
            shard::dynamic_bitset bits(8, 0b0011'1100);
            shard::dynamic_bitset mask(8, 0b1100'0011);

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
            shard::dynamic_bitset bits(8, 0b1111'1111);
            shard::dynamic_bitset mask(8, 0b0011'1100);

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
            shard::dynamic_bitset bits(8);
            REQUIRE(bits.none());
            bits = ~bits;
            REQUIRE(bits.all());
        }

        SUBCASE("equality") {
            shard::dynamic_bitset bits_1(8, 0b1010'1010);
            shard::dynamic_bitset bits_2(8, 0b1010'1010);
            shard::dynamic_bitset bits_3(8, 0b0101'0101);
            shard::dynamic_bitset bits_4(16, 0b1010'1010);

            REQUIRE(bits_1 == bits_2);
            REQUIRE_FALSE(bits_1 == bits_3);
            REQUIRE_FALSE(bits_1 == bits_4); // different sizes
        }

        SUBCASE("across block boundaries") {
            shard::dynamic_bitset bits_1(64);
            shard::dynamic_bitset bits_2(64);

            // set different patterns
            for (auto i = 0ul; i < 32; ++i) {
                bits_1.set(i);      // first block all set
                bits_2.set(32 + i); // second block all set
            }

            auto bits_or = bits_1 | bits_2;
            REQUIRE(bits_or.all());

            auto bits_and = bits_1 & bits_2;
            REQUIRE(bits_and.none());

            auto bits_xor = bits_1 ^ bits_2;
            REQUIRE(bits_xor.all());
        }
    }

    SUBCASE("iteration") {
        shard::dynamic_bitset bits(8);
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
        shard::dynamic_bitset bits(8);
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

    SUBCASE("find") {
        SUBCASE("empty and no bits set") {
            shard::dynamic_bitset empty;
            REQUIRE(empty.find_first() == shard::dynamic_bitset<>::npos);

            shard::dynamic_bitset all_clear(64);
            REQUIRE(all_clear.find_first() == shard::dynamic_bitset<>::npos);
        }

        SUBCASE("only last bit set") {
            shard::dynamic_bitset bits(100);
            bits.set(99);
            REQUIRE(bits.find_first() == 99);
            REQUIRE(bits.find_next(99) == shard::dynamic_bitset<>::npos);
        }

        SUBCASE("bits in different blocks") {
            shard::dynamic_bitset bits(128);
            bits.set(10).set(50).set(90).set(120);

            REQUIRE(bits.find_first() == 10);
            REQUIRE(bits.find_next(10) == 50);
            REQUIRE(bits.find_next(50) == 90);
            REQUIRE(bits.find_next(90) == 120);
            REQUIRE(bits.find_next(120) == shard::dynamic_bitset<>::npos);
        }

        SUBCASE("consecutive bits") {
            shard::dynamic_bitset bits(64);
            bits.set(30).set(31).set(32).set(33); // across block boundary

            REQUIRE(bits.find_first() == 30);
            REQUIRE(bits.find_next(30) == 31);
            REQUIRE(bits.find_next(31) == 32);
            REQUIRE(bits.find_next(32) == 33);
        }
    }

    SUBCASE("push_back / pop_back") {
        shard::dynamic_bitset bits(31); // one less than block size
        REQUIRE(bits.num_blocks() == 1);

        bits.push_back(true);
        REQUIRE(bits.num_blocks() == 1);
        REQUIRE(bits.size() == 32);
        REQUIRE(bits.test(31));

        bits.push_back(true);
        REQUIRE(bits.num_blocks() == 2); // crossed boundary
        REQUIRE(bits.size() == 33);
        REQUIRE(bits.test(32));

        bits.pop_back();
        REQUIRE(bits.num_blocks() == 1); // back to one block
        REQUIRE(bits.size() == 32);

        bits.pop_back();
        REQUIRE(bits.num_blocks() == 1);
        REQUIRE(bits.size() == 31);
    }

    SUBCASE("swap") {
        shard::dynamic_bitset bits_1(8, 0b1111'0000);
        shard::dynamic_bitset bits_2(16, 0b1010'1010'1010'1010);

        auto bits1_copy = bits_1;
        auto bits2_copy = bits_2;

        bits_1.swap(bits_2);

        REQUIRE(bits_1 == bits2_copy);
        REQUIRE(bits_2 == bits1_copy);
        REQUIRE(bits_1.size() == 16);
        REQUIRE(bits_2.size() == 8);
    }

    SUBCASE("all") {
        SUBCASE("partial last block") {
            shard::dynamic_bitset bits(40); // 2 blocks, last one partial
            bits.set();
            REQUIRE(bits.all());
            REQUIRE(bits.count() == 40);

            bits.reset(39);
            REQUIRE_FALSE(bits.all());

            bits.set(39);
            REQUIRE(bits.all());
        }

        SUBCASE("full blocks") {
            shard::dynamic_bitset bits_full(64);
            bits_full.set();
            REQUIRE(bits_full.all());

            bits_full.reset(0);
            REQUIRE_FALSE(bits_full.all());
        }
    }

    SUBCASE("resize") {
        shard::dynamic_bitset bits(32);
        bits.set(0).set(10).set(20).set(31);
        REQUIRE(bits.count() == 4);

        // grow with true
        bits.resize(64, true);
        REQUIRE(bits.count() == 4 + 32); // original 4 + 32 new bits

        // check original bits preserved
        REQUIRE(bits.test(0));
        REQUIRE(bits.test(10));
        REQUIRE(bits.test(20));
        REQUIRE(bits.test(31));

        // check new bits set
        for (auto i = 32ul; i < 64; ++i) {
            REQUIRE(bits.test(i));
        }
    }

    SUBCASE("test_set") {
        shard::dynamic_bitset bits(8);

        // test_set on false bit
        REQUIRE_FALSE(bits.test_set(0, true));
        REQUIRE(bits.test(0));

        // test_set on true bit with same value
        REQUIRE(bits.test_set(0, true));
        REQUIRE(bits.test(0));

        // test_set on true bit with different value
        REQUIRE(bits.test_set(0, false));
        REQUIRE_FALSE(bits.test(0));

        // test_set on false bit with false
        REQUIRE_FALSE(bits.test_set(0, false));
        REQUIRE_FALSE(bits.test(0));
    }

    SUBCASE("sanitization") {
        shard::dynamic_bitset bits(40); // partial last block

        bits.set();
        REQUIRE(bits.count() == 40);
        REQUIRE(bits.all());

        bits.flip();
        REQUIRE(bits.count() == 0);
        REQUIRE(bits.none());

        bits.resize(50, true);
        REQUIRE(bits.count() == 10); // only new bits are set
    }
}
