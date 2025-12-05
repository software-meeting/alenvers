// match_char.hpp
// helpers for identifying lexemes

#pragma once

#include <cctype>
#include <cwctype>
namespace match_char {

template <const char... match>
auto match_char(const char c) -> bool {
    return ((c == match) || ...);
}

auto is_delimiter(const char c) -> bool {
    return std::iswspace(c) || match_char<'(', ')', '"', ';'>(c);
}

auto is_special_initial(const char c) -> bool {
    return match_char<'!', '$', '%', '&', '*', '/', ':', '<', '=', '>', '?', '@', '^', '_', '~'>(c);
}

auto is_initial(const char c) -> bool { return std::isalpha(c) || is_special_initial(c); }

auto is_explicit_sign(const char c) -> bool { return match_char<'+', '-'>(c); }

auto is_special_subsequent(const char c) -> bool { return is_explicit_sign(c) || match_char<'.', '@'>(c); }

auto is_subsequent(const char c) -> bool {
    return is_initial(c) || std::isdigit(c) || is_special_subsequent(c);
}

}  // namespace match_char
