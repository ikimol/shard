// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/containers/array.hpp>
#include <shard/math/prime.hpp>
#include <shard/memory/allocator.hpp>
#include <shard/utility/preprocessor.hpp>

#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <stdexcept>

namespace shard {
namespace containers {
namespace detail {

// convert the hash into an index using the number of buckets
std::size_t constrain_hash(std::size_t h, std::size_t bc) {
    return !(bc & (bc - 1)) ? h & (bc - 1) : (h < bc ? h : h % bc);
}

} // namespace detail

template <typename K, typename T, typename Hash = std::hash<K>, typename KeyEqual = std::equal_to<K>>
class hash_map {
public:
    using key_type = K;
    using mapped_type = T;

private:
    struct entry {
        entry() = default;
        entry(const entry& other) = default;
        entry(entry&& other) noexcept = default;

        /// Forwarding key constructor
        template <typename U, typename = std::enable_if_t<!std::is_same_v<U, entry>>>
        explicit entry(U&& key)
        : key(std::forward<U>(key)) {}

        /// Forwarding key & value constructor
        template <typename U, typename V>
        entry(U&& key, V&& value)
        : key(std::forward<U>(key))
        , value(std::forward<V>(value)) {}

        const key_type key;
        mapped_type value;
    };

public:
    using value_type = entry;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

private:
    struct node {
        node() = default;
        node(const node& other) = default;
        node(node&& other) noexcept = default;

        /// Forwarding constructor
        template <typename... Args>
        explicit node(Args&&... args)
        : entry(std::forward<Args>(args)...) {}

        value_type entry;
        node* next = nullptr;
    };

    template <typename E>
    class basic_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = E;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

    public:
        basic_iterator() = default;

        explicit basic_iterator(hash_map* map, bool end = false) noexcept
        : m_map(map) {
            if (end) {
                m_bucket = m_map->bucket_count();
            } else {
                m_node = m_map->m_buckets[0];
                while (!m_node && ++m_bucket < map->bucket_count()) {
                    m_node = m_map->m_buckets[m_bucket];
                }
            }
        }

        basic_iterator(hash_map* m, node* n, size_type b) noexcept
        : m_map(m)
        , m_node(n)
        , m_bucket(b) {}

        basic_iterator(const basic_iterator& other) {
            m_map = other.m_map;
            m_bucket = other.m_bucket;
            m_node = other.m_node;
        }

        basic_iterator& operator++() {
            m_node = m_node->next;
            while (!m_node && ++m_bucket < m_map->bucket_count()) {
                m_node = m_map->m_buckets[m_bucket];
            }
            return *this;
        }

        basic_iterator operator++(int) {
            basic_iterator copy(*this);
            operator++();
            return copy;
        }

        bool operator==(const basic_iterator& rhs) const {
            return m_map == rhs.m_map && m_bucket == rhs.m_bucket && m_node == rhs.m_node;
        }

        bool operator!=(const basic_iterator& rhs) const { return !(*this == rhs); }

        value_type* operator->() { return &m_node->entry; }
        const value_type* operator->() const { return &m_node->entry; }

        value_type& operator*() { return m_node->entry; }
        const value_type& operator*() const { return m_node->entry; }

    private:
        hash_map* m_map = nullptr;
        node* m_node = nullptr;
        size_type m_bucket = 0;
    };

public:
    using iterator = basic_iterator<value_type>;
    using const_iterator = basic_iterator<const value_type>;

public:
    /// Create a hash map without allocating any memory
    explicit hash_map(allocator& a)
    : m_allocator(&a)
    , m_buckets(a) {}

    /// Create a new hash map and allocate memory for \a bc buckets
    hash_map(allocator& a, size_type bc)
    : m_allocator(&a)
    , m_buckets(a, bc) {}

    /// Copy constructor
    hash_map(const hash_map& other)
    : m_allocator(other.m_allocator)
    , m_hasher(other.m_hasher)
    , m_key_equal(other.m_key_equal)
    , m_buckets(other.m_buckets)
    , m_size(other.m_size)
    , m_max_load_factor(other.m_max_load_factor) {}

    /// Move constructor
    hash_map(hash_map&& other) noexcept
    : m_allocator(other.m_allocator)
    , m_hasher(std::move(other.m_hasher))
    , m_key_equal(std::move(other.m_key_equal))
    , m_buckets(std::move(other.m_buckets))
    , m_size(other.m_size)
    , m_max_load_factor(other.m_max_load_factor) {
        other.m_allocator = nullptr;
        other.m_size = 0;
        other.m_max_load_factor = 0.f;
    }

    /// Destroy the hash map, effectively destroying every element and
    /// deallocating memory
    ~hash_map() { clear(); }

    /// Copy assignment operator
    hash_map& operator=(const hash_map& other) {
        // FIXME: Not implemented
        assert(false && "not implemented");
        if (this != &other) {}
        return *this;
    }

    /// Move assignment operator
    hash_map& operator=(hash_map&& other) noexcept {
        // FIXME: Not implemented
        assert(false && "not implemented");
        return *this;
    }

    // modifiers

    /// Add a new element to the map with a key and a value
    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        auto bc = m_buckets.size();
        auto& key = first_arg(std::forward<Args>(args)...);
        if (bc != 0) {
            auto b = detail::constrain_hash(m_hasher(key), bc);
            if (auto node = get_node(b, key)) {
                return std::make_pair(iterator(this, node, b), false);
            }
        }
        if ((m_size + 1 > bc * m_max_load_factor) || bc == 0) {
            rehash(math::next_prime(m_buckets.size() * 2));
            bc = m_buckets.size();
        }
        auto b = detail::constrain_hash(m_hasher(key), bc);
        auto n = new_object<node>(*m_allocator, std::forward<Args>(args)...);
        n->next = m_buckets[b];
        m_buckets[b] = n;
        ++m_size;
        return std::make_pair(iterator(this, n, b), true);
    }

    // iterator remove(const_iterator pos) {}

    /// Remove the element with the given key
    bool remove(const key_type& key) {
        auto b = detail::constrain_hash(m_hasher(key), m_buckets.size());
        node* parent;
        auto n = get_node(b, key, &parent);
        if (n) {
            if (!parent) {
                m_buckets[b] = n->next;
            } else {
                parent->next = n->next;
            }
            delete_object(*m_allocator, n);
            --m_size;
            return true;
        }
        return false;
    }

    /// Clear the hash map, destroying every element
    void clear() noexcept {
        for (auto& node : m_buckets) {
            while (node != nullptr) {
                auto next = node->next;
                shard::delete_object(*m_allocator, node);
                node = next;
            }
        }
        m_size = 0;
    }

    /// Exchange the contents of the array with those of another
    void swap(hash_map& other) {
        using std::swap;
        swap(m_allocator, other.m_allocator);
        swap(m_hasher, other.m_hasher);
        swap(m_key_equal, other.m_key_equal);
        swap(m_buckets, other.m_buckets);
        swap(m_size, other.m_size);
        swap(m_max_load_factor, other.m_max_load_factor);
    }

    // lookup

    /// Get the value with the given key
    ///
    /// \note Will check if the key exists and throw if it does not
    mapped_type& get(const key_type& key) {
        auto& const_this = static_cast<const hash_map&>(*this);
        return const_cast<mapped_type&>(const_this.get(key));
    }

    /// Get the value with the given key
    ///
    /// \note Will check if the key exists and throw if it does not
    const mapped_type& get(const key_type& key) const {
        if (auto node = safe_get_node(key)) {
            return node->entry.value;
        }
        throw std::out_of_range("shard::hash_map::get: key not found");
    }

    /// Get or insert a value with the given key
    mapped_type& operator[](const key_type& key) { return (*emplace(key).first).value; }

    /// Check if a value exists with the given key
    bool contains(const key_type& key) const { return safe_get_node(key) != nullptr; }

    // size & capacity

    /// Get the number of elements
    size_type size() const { return m_size; }

    /// Check if the hash map is empty
    bool is_empty() const { return m_size == 0; }

    // for STL compatibility
    bool empty() const { return is_empty(); }

    // bucket interface

    /// Get the number of buckets
    size_type bucket_count() const { return m_buckets.size(); }

    /// Get the index of the bucket the key belongs to
    ///
    /// \note The returned value is valid only for instances of the container
    ///       for which bucket_count() returns the same value.
    ///
    /// \warning The behavior is undefined if bucket_count() is zero.
    size_type bucket(const key_type& key) const { return detail::constrain_hash(m_hasher(key), m_buckets.size()); }

    /// Get the number of elements in the bucket with index \a bucket
    size_type bucket_size(size_type bucket) const {
        size_type count = 0;
        auto node = m_buckets[bucket];
        while (node) {
            ++count;
            node = node->next;
        }
        return count;
    }

    /// Get the number of buckets memory is reserved for
    size_type capacity() const { return m_buckets.capacity(); }

    // hash policy

    /// Get the average number of elements per bucket
    float load_factor() const { return !m_buckets.is_empty() ? (float)(m_size) / m_buckets.size() : 0.f; }

    /// Get current maximum load factor
    float max_load_factor() const { return m_max_load_factor; }

    /// Set the maximum load factor
    void set_max_load_factor(float f) { m_max_load_factor = f; }

    /// Set the number of buckets to \a n and rehash the container
    void rehash(size_type n) { m_buckets.resize(n); }

    /// Set the number of buckets to the number needed to accommodate at least
    /// \a n elements without exceeding the maximum load factor and rehash the
    /// container
    void reserve(size_type n) { rehash(static_cast<size_type>(std::ceil((float)(n) / m_max_load_factor))); }

    // iterators

    iterator begin() noexcept { return iterator(this); }

    const_iterator begin() const noexcept { return const_iterator(this); }

    const_iterator cbegin() const noexcept { return const_iterator(this); }

    iterator end() noexcept { return iterator(this, true); }

    const_iterator end() const noexcept { return const_iterator(this, true); }

    const_iterator cend() const noexcept { return const_iterator(this, true); }

private:
    node* get_node(std::size_t bucket, const key_type& key, node** parent = nullptr) const {
        node* p = nullptr;
        auto node = m_buckets[bucket];
        while (node && !m_key_equal(key, node->entry.key)) {
            p = node;
            node = node->next;
        }
        if (parent) {
            *parent = p;
        }
        return node;
    }

    node* safe_get_node(const key_type& key) const {
        auto bc = m_buckets.size();
        if (bc != 0) {
            auto b = detail::constrain_hash(m_hasher(key), bc);
            return get_node(b, key);
        }
        return nullptr;
    }

    template <typename U, typename... Args>
    const U& first_arg(const U& first, Args&&...) {
        return first;
    }

private:
    allocator* m_allocator;

    hasher m_hasher;
    key_equal m_key_equal;

    array<node*> m_buckets;
    size_type m_size = 0;
    float m_max_load_factor = 1.f;
};

template <typename K, typename T, typename Hash, typename KeyEqual>
void swap(hash_map<K, T, Hash, KeyEqual>& lhs, hash_map<K, T, Hash, KeyEqual>& rhs) {
    lhs.swap(rhs);
}

template <typename K, typename T, typename Hash, typename KeyEqual>
bool operator==(const hash_map<K, T, Hash, KeyEqual>& lhs, const hash_map<K, T, Hash, KeyEqual>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    using const_iterator = typename hash_map<K, T, Hash, KeyEqual>::const_iterator;
    for (const_iterator i = lhs.begin(), e_lhs = lhs.end(), e_rhs = rhs.end(); i != e_lhs; ++i) {
        auto j = rhs.find(i->first);
        if (j == e_rhs || !(*i == *j)) {
            return false;
        }
    }
    return true;
}

template <typename K, typename T, typename Hash, typename KeyEqual>
bool operator!=(const hash_map<K, T, Hash, KeyEqual>& lhs, const hash_map<K, T, Hash, KeyEqual>& rhs) {
    return !(lhs == rhs);
}

} // namespace containers

// bring symbols into parent namespace

using containers::hash_map;

} // namespace shard
