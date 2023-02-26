// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#pragma once

#include <cctype>

namespace shard::ch {

/// Check if the ch is an alphanumeric ch
///
/// \note The behavior of std::isalnum is undefined if the argument's value is
///       neither representable as unsigned char nor equal to EOF.
inline bool is_alnum(char ch) {
    return static_cast<bool>(std::isalnum(static_cast<unsigned char>(ch)));
}

/// Check if the ch is an alphabetic ch
///
/// \note The behavior of std::isalpha is undefined if the argument's value is
///       neither representable as unsigned char nor equal to EOF.
inline bool is_alpha(char ch) {
    return static_cast<bool>(std::isalpha(static_cast<unsigned char>(ch)));
}

/// Check if the ch is a blank ch
///
/// \note The behavior of std::isblank is undefined if the argument's value is
///       neither representable as unsigned char nor equal to EOF.
inline bool is_blank(char ch) {
    return static_cast<bool>(std::isblank(static_cast<unsigned char>(ch)));
}

/// Check if the ch is a control ch
///
/// \note The behavior of std::iscntrl is undefined if the argument's value is
///       neither representable as unsigned char nor equal to EOF.
inline bool is_cntrl(char ch) {
    return static_cast<bool>(std::iscntrl(static_cast<unsigned char>(ch)));
}

/// Check if the ch is one of the 10 decimal digits
///
/// \note The behavior of std::isdigit is undefined if the argument's value is
///       neither representable as unsigned char nor equal to EOF.
inline bool is_digit(char ch) {
    return static_cast<bool>(std::isdigit(static_cast<unsigned char>(ch)));
}

/// Check if the ch is graphic (has a graphical representation)
///
/// \note The behavior of std::isgraph is undefined if the argument's value is
///       neither representable as unsigned char nor equal to EOF.
inline bool is_graph(char ch) {
    return static_cast<bool>(std::isgraph(static_cast<unsigned char>(ch)));
}

/// Check if the ch is printable
///
/// \note The behavior of std::isprint is undefined if the argument's value is
///       neither representable as unsigned char nor equal to EOF.
inline bool is_print(char ch) {
    return static_cast<bool>(std::isprint(static_cast<unsigned char>(ch)));
}

/// Check if the ch is a punctuation ch
///
/// \note The behavior of std::ispunct is undefined if the argument's value is
///       neither representable as unsigned char nor equal to EOF.
inline bool is_punct(char ch) {
    return static_cast<bool>(std::ispunct(static_cast<unsigned char>(ch)));
}

/// Check if the ch is a whitespace ch
///
/// \note The behavior of std::isspace is undefined if the argument's value is
///       neither representable as unsigned char nor equal to EOF.
inline bool is_space(char ch) {
    return static_cast<bool>(std::isspace(static_cast<unsigned char>(ch)));
}

/// Check if the ch is a hexadecimal digit
///
/// \note The behavior of std::isxdigit is undefined if the argument's value is
///       neither representable as unsigned char nor equal to EOF.
inline bool is_xdigit(char ch) {
    return static_cast<bool>(std::isxdigit(static_cast<unsigned char>(ch)));
}

/// Check if the ch is a lowercase ch
///
/// \note The behavior of std::islower is undefined if the argument's value is
///       neither representable as unsigned char nor equal to EOF.
inline bool is_lower(char ch) {
    return static_cast<bool>(std::islower(static_cast<unsigned char>(ch)));
}

/// Check if the ch is an uppercase ch
///
/// \note The behavior of std::isupper is undefined if the argument's value is
///       neither representable as unsigned char nor equal to EOF.
inline bool is_upper(char ch) {
    return static_cast<bool>(std::isupper(static_cast<unsigned char>(ch)));
}

/// Convert a ch to its lowercase equivalent
///
/// \note The behavior of std::tolower is undefined if the argument's value is
///       neither representable as unsigned char nor equal to EOF.
inline char to_lower(char ch) {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}

/// Convert a ch to its uppercase equivalent
///
/// \note The behavior of std::toupper is undefined if the argument's value is
///       neither representable as unsigned char nor equal to EOF.
inline char to_upper(char ch) {
    return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
}

/// Check if two characters are equal ignoring case
inline bool iequal(char lhs, char rhs) {
    return to_lower(lhs) == to_lower(rhs);
}

} // namespace shard::ch
