// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#pragma once

#include <shard/meta/type_traits.hpp>

#include <array>
#include <cstddef>
#include <iterator>
#include <type_traits>

namespace shard {
namespace utility {
namespace detail {

template <typename, typename = void>
struct is_iterator : std::false_type {};

template <typename Iterator>
struct is_iterator<Iterator, std::void_t<typename std::iterator_traits<Iterator>::pointer>> : std::true_type {};

template <typename Iterator>
inline constexpr bool is_iterator_v = is_iterator<Iterator>::value;

template <typename, typename, typename = void>
struct is_range : std::false_type {};

template <typename R, typename T>
struct is_range<R,
                T,
                std::void_t<decltype(static_cast<T*>(std::declval<R&>().data())), decltype(std::declval<R&>().size())>>
: std::true_type {};

template <typename R, typename T>
inline constexpr bool is_compatible_range_v = is_range<R, T>::value;

} // namespace detail

/// Used to differentiate static and dynamic extent in spans
constexpr std::size_t dynamic_extent = static_cast<std::size_t>(-1);

/// A non-owning view of contiguous memory
template <typename T>
class span {
public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using index_type = std::size_t;
    using size_type = std::size_t;
    using pointer = T*;
    using const_pointer = const value_type*;
    using reference = T&;
    using const_reference = const value_type&;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    template <typename Iterator, typename = std::enable_if_t<detail::is_iterator_v<Iterator>>>
    constexpr span(Iterator first, std::size_t size) noexcept
    : m_base(std::addressof(*first))
    , m_size(size) {}

    template <typename Iterator, typename = std::enable_if_t<detail::is_iterator_v<Iterator>>>
    constexpr span(Iterator first, Iterator last) noexcept
    : m_base(std::addressof(*first))
    , m_size(std::distance(first, last)) {}

    template <std::size_t N>
    constexpr /* implicit */ span(T (&array)[N]) noexcept /* NOLINT */
    : m_base(std::data(array))
    , m_size(N) {}

    template <typename U, std::size_t N>
    constexpr /* implicit */ span(std::array<U, N>& array) noexcept /* NOLINT*/
    : m_base(std::data(array))
    , m_size(N) {}

    template <typename U, std::size_t N>
    constexpr /* implicit */ span(const std::array<U, N>& array) noexcept /* NOLINT*/
    : m_base(std::data(array))
    , m_size(N) {}

    template <typename R,
              typename = std::enable_if_t<detail::is_compatible_range_v<R, T> && !std::is_array_v<unqualified_t<R>>
                                          && !std::is_same_v<unqualified_t<R>, span>>>
    constexpr /* implicit */ span(R& r) noexcept /* NOLINT */
    : m_base(std::data(r))
    , m_size(std::size(r)) {}

    constexpr reference operator[](size_type index) const { return m_base[index]; }

    constexpr reference at(size_type index) const {
        if (index >= m_size) {
            throw std::out_of_range("shard::span::at()");
        }
        return m_base[index];
    }

    constexpr pointer data() const noexcept { return m_base; }

    constexpr reference front() const { return m_base[0]; }

    constexpr reference back() const { return m_base[m_size - 1]; }

    size_type size() const { return m_size; }

    bool empty() const { return m_size == 0; }

    // sup-spans

    constexpr span first(size_type count) const { return {m_base, count}; }

    constexpr span last(size_type count) const { return {m_base + (m_size - count), count}; }

    constexpr span subspan(size_type offset, size_type count = dynamic_extent) {
        return {m_base + offset, count == dynamic_extent ? m_size - offset : count};
    }

    // iterators

    iterator begin() noexcept { return m_base; }

    const_iterator begin() const noexcept { return m_base; }

    const_iterator cbegin() const noexcept { return m_base; }

    iterator end() noexcept { return m_base + m_size; }

    const_iterator end() const noexcept { return m_base + m_size; }

    const_iterator cend() const noexcept { return m_base + m_size; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }

    const_reverse_iterator crbegin() const noexcept { return rbegin(); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

    const_reverse_iterator crend() const noexcept { return rend(); }

private:
    pointer m_base = nullptr;
    size_type m_size = 0;
};

// deduction guides

template <typename Iterator>
span(Iterator, std::size_t) -> span<std::remove_reference_t<decltype(*std::declval<Iterator>())>>;

template <typename Iterator>
span(Iterator, Iterator) -> span<std::remove_reference_t<decltype(*std::declval<Iterator>())>>;

template <typename T, std::size_t N>
span(std::array<T, N>&) -> span<T>;

template <typename T, std::size_t N>
span(const std::array<T, N>&) -> span<const T>;

template <typename R>
span(R&) -> span<std::remove_reference_t<decltype(*std::data(std::declval<R&>()))>>;

} // namespace utility

// bring symbols into parent namespace

using utility::span;

} // namespace shard
