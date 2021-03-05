// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#ifndef SHARD_ALGORITHM_PAIRS_HPP
#define SHARD_ALGORITHM_PAIRS_HPP

#include <cassert>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>

namespace shard {
namespace algorithm {
namespace detail {

template <typename T, template <typename> class E>
class element_iterator;

template <typename T>
class map_element {
    friend class element_iterator<T, map_element>;

public:
    using iterator = decltype(std::begin(std::declval<T&>()));
    using key_type = std::remove_reference_t<decltype(std::begin(std::declval<T&>())->first)>;
    using mapped_type = std::remove_reference_t<decltype(std::begin(std::declval<T&>())->second)>;

public:
    map_element() = default;

    map_element(std::size_t index, iterator iterator) : m_index(index), m_iterator(iterator) {}

    map_element& operator=(const map_element& other) {
        if (this != &other) {
            m_index = other.m_index;
            m_iterator = other.m_iterator;
        }
        return *this;
    }

    std::size_t index() const { return m_index; }

    const key_type& key() const { return m_iterator->first; }

    mapped_type& value() { return m_iterator->second; }

    const mapped_type& value() const { return m_iterator->second; }

private:
    std::size_t m_index = std::numeric_limits<std::size_t>::max();
    iterator m_iterator;
};

template <typename T>
class array_element {
    friend class element_iterator<T, array_element>;

public:
    using iterator = decltype(std::begin(std::declval<T&>()));
    using value_type = std::remove_reference_t<decltype(*std::begin(std::declval<T&>()))>;

public:
    array_element() = default;

    array_element(std::size_t index, iterator iterator) : m_index(index), m_iterator(iterator) {}

    array_element& operator=(const array_element& other) {
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

template <typename T, template <typename> class E>
class element_iterator : public std::iterator<std::forward_iterator_tag, E<T>> {
public:
    using iterator = decltype(std::begin(std::declval<T&>()));
    using result_type = E<T>;

public:
    explicit element_iterator(iterator end) : m_result(std::numeric_limits<size_t>::max(), end) {}

    element_iterator(std::size_t index, iterator it) : m_result(index, it) {}

    result_type& operator*() { return m_result; }

    const result_type& operator*() const { return m_result; }

    element_iterator& operator++() {
        assert(m_result.m_index != std::numeric_limits<size_t>::max());
        ++m_result.m_index;
        ++m_result.m_iterator;
        return *this;
    }

    bool operator==(const element_iterator& other) const {
        // don't compare indices here, only iterators
        // it is possible for an end iterator to have different indices
        // depending on whether it was created by calling std::end() or
        // incrementing a valid iterator
        return m_result.m_iterator == other.m_result.m_iterator;
    }

    bool operator!=(const element_iterator& other) const { return !(*this == other); }

    element_iterator& operator=(const element_iterator& other) {
        if (this != &other) {
            m_result = other.m_result;
        }
        return *this;
    }

private:
    result_type m_result;
};

template <typename T, template <typename> class E>
class element_range {
public:
    explicit element_range(T&& range) : m_range(std::forward<T>(range)) {}

    element_iterator<T, E> begin() { return element_iterator<T, E>(0, std::begin(m_range)); }

    element_iterator<T, E> end() { return element_iterator<T, E>(std::end(m_range)); }

private:
    T m_range;
};

} // namespace detail

template <typename T>
detail::element_range<T, detail::map_element> pairs(T&& range) {
    return detail::element_range<T, detail::map_element>(std::forward<T>(range));
}

template <typename T>
detail::element_range<T, detail::array_element> ipairs(T&& range) {
    return detail::element_range<T, detail::array_element>(std::forward<T>(range));
}

} // namespace algorithm

// bring symbols into parent namespace

using algorithm::ipairs;
using algorithm::pairs;

} // namespace shard

#endif // SHARD_ALGORITHM_PAIRS_HPP
