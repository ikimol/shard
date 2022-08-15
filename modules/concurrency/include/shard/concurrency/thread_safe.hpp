// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_CONCURRENCY_SYNC_HPP
#define SHARD_CONCURRENCY_SYNC_HPP

#include "shard/concurrency/lock_traits.hpp"

#include <shard/meta/type_traits.hpp>

#include <utility>

namespace shard {
namespace concurrency {
namespace detail {

template <typename T>
struct reference_or_mutable {
    mutable T value;
};

template <typename T>
struct reference_or_mutable<T&> {
    T& value;
};

} // namespace detail

enum class access_mode { read_only, read_write };

/// Represents a thread-safe wrapper of a type
template <typename T, typename Mutex>
class basic_thread_safe {
public:
    using mutex_type = Mutex;
    using value_type = std::decay_t<T>;
    using reference = value_type&;
    using const_reference = const value_type&;

private:
    /// RAII locked access wrapper to a pointer
    template <template <typename> class Lock, access_mode t_mode>
    class access {
        static_assert(!(lock_traits<Lock>::is_read_only && t_mode == access_mode::read_write),
        "read-only lock with read-write access");

        using const_correct_value_type =
        std::conditional_t<t_mode == access_mode::read_only, const value_type, value_type>;

    public:
        using lock_type = Lock<mutex_type>;
        using pointer = const_correct_value_type*;
        using const_pointer = const const_correct_value_type*;
        using reference = const_correct_value_type&;
        using const_reference = const const_correct_value_type&;

    public:
        template <typename... LockArgs>
        access(reference value, mutex_type& mutex, LockArgs&&... lock_args) :
        lock(mutex, std::forward<LockArgs>(lock_args)...),
        m_pointer(&value) {}

        template <typename... LockArgs>
        access(const basic_thread_safe& sync, LockArgs&&... lock_args) /* NOLINT */ :
        access(sync.m_value, sync.m_mutex.value, std::forward<LockArgs>(lock_args)...) {}

        template <typename... LockArgs>
        access(basic_thread_safe& sync, LockArgs&&... lock_args) /* NOLINT */ :
        access(sync.m_value, sync.m_mutex.value, std::forward<LockArgs>(lock_args)...) {}

        template <template <typename> class OtherLock, access_mode t_other_mode, typename... LockArgs>
        access(access<OtherLock, t_other_mode>& other, LockArgs&&... lock_args) /* NOLINT */ :
        access(*other, *other.lock.release(), std::adopt_lock, std::forward<LockArgs>(lock_args)...) {
            static_assert(t_other_mode == access_mode::read_write || t_other_mode == t_mode,
            "cannot construct read-write access from read-only");
        }

        pointer operator->() noexcept { return m_pointer; }

        const_pointer operator->() const noexcept { return m_pointer; }

        reference operator*() noexcept { return *m_pointer; }

        const_reference operator*() const noexcept { return *m_pointer; }

    public:
        mutable lock_type lock;

    private:
        pointer m_pointer;
    };

public:
    template <template <typename> class Lock = std::shared_lock>
    using read_access = access<Lock, access_mode::read_only>;

    template <template <typename> class Lock = std::lock_guard>
    using write_access = access<Lock, access_mode::read_write>;

public:
    /// Default constructor
    basic_thread_safe() = default;

    basic_thread_safe(const basic_thread_safe&) = delete;
    basic_thread_safe(basic_thread_safe&&) = delete;

    template <typename... Args>
    explicit basic_thread_safe(Args&&... args) : m_value(std::forward<Args>(args)...) {}

    basic_thread_safe& operator=(const basic_thread_safe&) = delete;
    basic_thread_safe& operator=(basic_thread_safe&&) = delete;

    template <template <typename> class Lock = std::lock_guard, typename... LockArgs>
    read_access<Lock> access_for_read(LockArgs&&... lock_args) const {
        return {*this, std::forward<LockArgs>(lock_args)...};
    }

    template <template <typename> class Lock = std::lock_guard, typename... LockArgs>
    write_access<Lock> access_for_write(LockArgs&&... lock_args) {
        return {*this, std::forward<LockArgs>(lock_args)...};
    }

    template <template <typename> class Lock = std::lock_guard, typename... LockArgs>
    value_type copy(LockArgs&&... lock_args) const {
        return *access_for_read<Lock>(std::forward<LockArgs>(lock_args)...);
    }

    template <template <typename> class Lock = std::lock_guard, typename... LockArgs>
    void assign(const value_type& value, LockArgs&&... lock_args) {
        *access_for_write<Lock>(std::forward<LockArgs>(lock_args)...) = value;
    }

    template <template <typename> class Lock = std::lock_guard, typename... LockArgs>
    void assign(value_type&& value, LockArgs&&... lock_args) {
        *access_for_write<Lock>(std::forward<LockArgs>(lock_args)...) = std::move(value);
    }

    /// Get an unsafe const reference to the value
    const_reference unsafe() const noexcept { return m_value; }

    /// Get an unsafe reference to the value
    reference unsafe() noexcept { return m_value; }

    /// Get a reference to the mutex
    mutex_type& mutex() const noexcept { return m_mutex.value; }

private:
    // the value to protect
    value_type m_value;

    // holds a reference to a mutex or a mutable mutex
    detail::reference_or_mutable<mutex_type> m_mutex;
};

template <typename T>
using thread_safe = basic_thread_safe<T, std::mutex>;

template <typename T>
using rw_thread_safe = basic_thread_safe<T, std::shared_mutex>;

template <typename ThreadSafe, template <typename> class Lock = std::shared_lock>
using read_access = typename ThreadSafe::template read_access<Lock>;

template <typename ThreadSafe, template <typename> class Lock = std::lock_guard>
using write_access = typename ThreadSafe::template write_access<Lock>;

} // namespace concurrency

// bring symbols into parent namespace

using concurrency::read_access;
using concurrency::rw_thread_safe;
using concurrency::thread_safe;
using concurrency::write_access;

} // namespace shard

#endif // SHARD_CONCURRENCY_SYNC_HPP
