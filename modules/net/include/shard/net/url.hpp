// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <regex>
#include <string>
#include <string_view>

namespace shard {
namespace net {

class url {
public:
    /// Default constructor
    url() = default;

    /// Copy constructor
    url(const url& other)
    : m_url(other.m_url) {
        parse();
    }

    /// Move constructor
    url(url&& other) noexcept
    : m_url(std::move(other.m_url)) {
        other.m_valid = false;
        parse();
    }

    /// Implicit value constructor from compatible type
    template <typename T, typename = std::enable_if_t<std::is_convertible<T, std::string>::value>>
    url(T&& value) /* NOLINT */
    : m_url(std::forward<T>(value)) {
        parse();
    }

    /// Copy assignment operator
    url& operator=(const url& other) {
        if (&other != this) {
            m_url = other.m_url;
            parse();
        }
        return *this;
    }

    /// Move assignment operator
    url& operator=(url&& other) noexcept {
        m_url = std::move(other.m_url);
        other.m_valid = false;
        parse();
        return *this;
    }

    /// Value assignment operator from compatible type
    template <typename T, typename = std::enable_if_t<std::is_convertible<T, std::string>::value>>
    url& operator=(T&& value) {
        m_url = std::forward<T>(value);
        parse();
        return *this;
    }

    /// Check if the URL parsing succeeded
    bool is_valid() const { return m_valid; }

    /// Retrieve the scheme part of the URL (e.g. 'http')
    std::string_view scheme() const { return m_scheme; }

    /// Retrieve the authority part of the URL (e.g. 'www.example.com')
    std::string_view authority() const { return m_authority; }

    /// Retrieve the path part of the URL (e.g. '/users/12345')
    std::string_view path() const { return m_path; }

    /// Retrieve the query part of the URL (e.g. 'from=home')
    std::string_view query() const { return m_query; }

    /// Retrieve the fragment part of the URL (e.g. the 'top' after a '#')
    std::string_view fragment() const { return m_fragment; }

    /// Get the string representation of this URL
    const std::string& to_string() const { return m_url; }

    /// Cast the URL to its string representation
    operator std::string() const /* NOLINT */ { return to_string(); }

    /// Swap the addresses and parse both new URLs
    void swap(url& other) {
        using std::swap;
        swap(m_url, other.m_url);
        parse();
        other.parse();
    }

private:
    std::string_view make_string_view(const std::smatch& groups, std::size_t index) {
        auto data = m_url.data() + groups.position(index);
        auto length = static_cast<std::string_view::size_type>(groups.length(index));
        return {data, length};
    }

    void parse() {
        // regex pattern from: https://www.ietf.org/rfc/rfc2396.txt
        static const std::regex uri_regex(R"(^(([^:\/?#]+):)?(\/\/([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)");
        std::smatch groups;

        auto match = std::regex_match(m_url, groups, uri_regex);
        if (match && !groups.empty()) {
            if (groups[2].matched) {
                m_scheme = make_string_view(groups, 2);
            }
            if (groups[4].matched) {
                m_authority = make_string_view(groups, 4);
            }
            if (groups[5].matched) {
                m_path = make_string_view(groups, 5);
            }
            if (groups[7].matched) {
                m_query = make_string_view(groups, 7);
            }
            if (groups[9].matched) {
                m_fragment = make_string_view(groups, 9);
            }
        }
        // scheme and authority or path must be set
        m_valid = match && ((!m_scheme.empty() && !m_authority.empty()) || !m_path.empty());
    }

private:
    bool m_valid = false;

    std::string m_url;

    std::string_view m_scheme;
    std::string_view m_authority;
    std::string_view m_path;
    std::string_view m_query;
    std::string_view m_fragment;
};

inline std::ostream& operator<<(std::ostream& os, const url& u) {
    os << u.to_string();
    return os;
}

} // namespace net

// bring symbols into parent namespace

using net::url;

} // namespace shard
