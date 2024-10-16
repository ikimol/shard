// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <cassert>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>

namespace shard {
namespace algorithm {
namespace detail {

template <typename T>
class enumerator;

template <typename T>
class enumerator_proxy {
    friend class enumerator<T>;

public:
    using iterator = decltype(std::begin(std::declval<T&>()));
    using value_type = std::remove_reference_t<decltype(*std::begin(std::declval<T&>()))>;

public:
    enumerator_proxy()
    : m_index(std::numeric_limits<std::size_t>::max())
    , m_iterator(nullptr) {}

    enumerator_proxy(std::size_t index, iterator iterator)
    : m_index(index)
    , m_iterator(iterator) {}

    enumerator_proxy(const enumerator_proxy& other)
    : m_index(other.m_index)
    , m_iterator(other.m_iterator) {}

    enumerator_proxy& operator=(const enumerator_proxy& other) {
        if (this != &other) {
            m_index = other.m_index;
            m_iterator = other.m_iterator;
        }
        return *this;
    }

    std::size_t index() const { return m_index; }

    value_type& value() { return *m_iterator; }

    const value_type& value() const { return *m_iterator; }

public: // structured binding support
    template <size_t N>
    auto& get() & {
        if constexpr (N == 0) {
            return m_index;
        } else if constexpr (N == 1) {
            return *m_iterator;
        }
    }

    template <size_t N>
    auto const& get() const& {
        if constexpr (N == 0) {
            return m_index;
        } else if constexpr (N == 1) {
            return *m_iterator;
        }
    }

    template <size_t N>
    auto&& get() && {
        if constexpr (N == 0) {
            return m_index;
        } else if constexpr (N == 1) {
            return std::move(*m_iterator);
        }
    }

private:
    std::size_t m_index;
    iterator m_iterator;
};

template <typename T>
class enumerator {
public: // std::iterator conformance
    using iterator_category = std::forward_iterator_tag;
    using value_type = enumerator_proxy<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

public:
    using result_type = enumerator_proxy<T>;
    using iterator_type = typename result_type::iterator;

public:
    explicit enumerator(iterator_type end)
    : m_result(std::numeric_limits<size_t>::max(), end) {}

    enumerator(std::size_t index, iterator_type iterator)
    : m_result(index, iterator) {}

    enumerator(const enumerator& other)
    : m_result(other.m_result) {}

    enumerator& operator=(const enumerator& other) {
        if (this != &other) {
            m_result = other.m_result;
        }
        return *this;
    }

    result_type& operator*() { return m_result; }

    const result_type& operator*() const { return m_result; }

    enumerator& operator++() {
        assert(m_result.m_index != std::numeric_limits<size_t>::max());
        ++m_result.m_iterator;
        ++m_result.m_index;
        return *this;
    }

    bool operator==(const enumerator& other) const {
        // don't compare indices here, only iterators
        // it's possible for an end iterator to have different indices depending on whether it was
        // created by calling std::end() versus incrementing a valid iterator
        return m_result.m_iterator == other.m_result.m_iterator;
    }

    bool operator!=(const enumerator& other) const { return !this->operator==(other); }

private:
    result_type m_result;
};

template <typename T>
class range_enumerator {
public:
    explicit range_enumerator(T&& range)
    : m_range(std::forward<T>(range)) {}

    enumerator<T> begin() { return enumerator<T>(0, std::begin(m_range)); }

    enumerator<T> end() { return enumerator<T>(std::end(m_range)); }

private:
    T m_range;
};

} // namespace detail

/// Get an iterable wrapper for the given range that unpacks both the index
/// and the value of the iterators.
template <typename T>
detail::range_enumerator<T> enumerate(T&& range) {
    return detail::range_enumerator<T>(std::forward<T>(range));
}

} // namespace algorithm

// bring symbols into parent namespace

using algorithm::enumerate;

} // namespace shard

// structured binding support

namespace std {

template <typename T>
struct tuple_size<shard::algorithm::detail::enumerator_proxy<T>> : std::integral_constant<size_t, 2> {};

template <typename T>
struct tuple_element<0, shard::algorithm::detail::enumerator_proxy<T>> {
    using type = size_t;
};

template <typename T>
struct tuple_element<1, shard::algorithm::detail::enumerator_proxy<T>> {
    using type = typename shard::algorithm::detail::enumerator_proxy<T>::value_type;
};

} // namespace std
