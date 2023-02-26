// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#ifndef SHARD_CONCURRENCY_TBUF_VALUE_HPP
#define SHARD_CONCURRENCY_TBUF_VALUE_HPP

#include <atomic>
#include <cstdint>

namespace shard {
namespace concurrency {

template <typename T>
class tbuf_value {
public:
    using value_type = T;

public:
    /// Get a copy of the value
    value_type load() {
        auto state = m_state.load(std::memory_order_acquire);
        auto load_index = (state & load_index_mask) >> load_index_shift;

        if ((state & has_ready_flag) != 0) {
            int temp_index;
            while (true) {
                temp_index = (state & temp_index_mask) >> temp_index_shift;
                auto new_state = state;
                new_state &= ~load_index_mask;               // unset load
                new_state &= ~temp_index_mask;               // unset temp
                new_state |= temp_index << load_index_shift; // load := temp
                new_state |= load_index << temp_index_shift; // temp := load
                new_state &= ~has_ready_flag;                // clear the ready flag

                if (!m_state.compare_exchange_weak(
                        state, new_state, std::memory_order_release, std::memory_order_acquire)) {
                    load_index = (state & load_index_mask) >> load_index_shift;
                } else {
                    load_index = temp_index;
                    break;
                }
            }
        }

        return m_value[load_index];
    }

    /// Store a value by perfect forwarding it
    template <typename U>
    void store(U&& value) {
        auto state = m_state.load(std::memory_order_acquire);
        auto store_index = (state & store_index_mask) >> store_index_shift;
        m_value[store_index] = std::forward<U>(value);

        uint_fast8_t new_state;
        do {
            auto temp_index = (state & temp_index_mask) >> temp_index_shift;
            new_state = state;
            new_state &= ~store_index_mask;               // unset store
            new_state &= ~temp_index_mask;                // unset temp
            new_state |= store_index << temp_index_shift; // temp := store
            new_state |= temp_index << store_index_shift; // store := temp
            new_state |= has_ready_flag;                  // set the ready flag
        } while (
            !m_state.compare_exchange_weak(state, new_state, std::memory_order_release, std::memory_order_acquire));
    }

private:
    constexpr static int store_index_mask = 0b00000011;
    constexpr static int store_index_shift = 0;

    constexpr static int temp_index_mask = 0b00001100;
    constexpr static int temp_index_shift = 2;

    constexpr static int load_index_mask = 0b00110000;
    constexpr static int load_index_shift = 4;

    constexpr static int has_ready_flag = 0b01000000;

private:
    T m_value[3] = {};
    std::atomic<std::uint_fast8_t> m_state =
        ATOMIC_VAR_INIT((0 << store_index_shift) | (1 << temp_index_shift) | (2 << temp_index_shift));
};

} // namespace concurrency

// bring symbols into parent namespace

using concurrency::tbuf_value;

} // namespace shard

#endif // SHARD_CONCURRENCY_TBUF_VALUE_HPP
