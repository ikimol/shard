// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_ALGORITHM_ENUMERATE_HPP
#define SHARD_ALGORITHM_ENUMERATE_HPP

#include <cassert>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>

namespace shard {
namespace algorithm {
namespace detail {

template <typename T>
using range_iter_type = decltype(std::begin(std::declval<T&>()));

template <typename T>
class enumerator;

template <typename T>
class enumerator_proxy {
    friend class enumerator<T>;

public:
    using iterator = range_iter_type<T>;
    using value_type = std::remove_reference_t<decltype(*std::begin(std::declval<T&>()))>;

    enumerator_proxy() = default;

    enumerator_proxy(std::size_t index, iterator iterator) : m_index(index), m_iterator(iterator) {}

    enumerator_proxy<T>& operator=(const enumerator_proxy<T>& other) {
        if (this != &other) {
            m_index = other.m_index;
            m_iterator = other.m_iterator;
        }
        return *this;
    }

    std::size_t index() const { return m_index; }

    value_type& value() { return *m_iterator; }

    const value_type& value() const { return *m_iterator; }

private:
    std::size_t m_index = std::numeric_limits<std::size_t>::max();
    iterator m_iterator;
};

template <typename T>
class enumerator : public std::iterator<std::forward_iterator_tag, enumerator_proxy<T>> {
public:
    using result_type = enumerator_proxy<T>;

public:
    explicit enumerator(range_iter_type<T> end) : m_result(std::numeric_limits<size_t>::max(), end) {}

    enumerator(std::size_t index, range_iter_type<T> iterator) : m_result(index, iterator) {}

    result_type& operator*() { return m_result; }

    const result_type& operator*() const { return m_result; }

    enumerator<T>& operator++() {
        assert(m_result.m_index != std::numeric_limits<size_t>::max());
        ++m_result.m_iterator;
        ++m_result.m_index;
        return *this;
    }

    bool operator==(const enumerator<T>& other) const {
        // don't compare indices here, only iterators
        // it's possible for an end iterator to have different indices depending on whether it was
        // created by calling std::end() versus incrementing a valid iterator
        return m_result.m_iterator == other.m_result.m_iterator;
    }

    bool operator!=(const enumerator<T>& other) const { return !this->operator==(other); }

    enumerator<T>& operator=(const enumerator<T>& other) {
        if (this != &other) {
            m_result = other.m_result;
        }
        return *this;
    }

private:
    result_type m_result;
};

template <typename T>
class range_enumerator {
public:
    explicit range_enumerator(T&& range) : m_range(std::forward<T>(range)) {}

    enumerator<T> begin() { return enumerator<T>(0, std::begin(m_range)); }

    enumerator<T> end() { return enumerator<T>(std::end(m_range)); }

private:
    T m_range;
};

} // namespace detail

template <typename T>
detail::range_enumerator<T> enumerate(T&& range) {
    return detail::range_enumerator<T>(std::forward<T>(range));
}

} // namespace algorithm

// bring symbols into parent namespace

using algorithm::enumerate;

} // namespace shard

#endif // SHARD_ALGORITHM_ENUMERATE_HPP
