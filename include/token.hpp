#pragma once

#include <concepts>
#include <cstdint>
#include <format>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>

namespace token {

template <typename T>
concept BaseToken = requires(T tok) {
    requires(std::same_as<std::remove_cvref_t<decltype(tok.line_number)>, uint_fast32_t>);
    requires(std::same_as<std::remove_cvref_t<decltype(tok.col_number)>, uint_fast32_t>);
};

template <typename T>
concept HasLexeme = requires(T tok) {
    requires(std::same_as<std::remove_cvref_t<decltype(tok.lexeme)>, std::string>);
};

template <typename T>
    requires BaseToken<T> && HasLexeme<T>
auto format_tok(const T& tok) -> std::string {
    return std::format("['{}', line: {}, column: {}]", tok.lexeme, tok.line_number, tok.col_number);
}

template <typename T>
    requires BaseToken<T>
auto format_tok(const T& tok, std::string_view lexeme) -> std::string {
    return std::format("['{}', line: {}, column: {}]", lexeme, tok.line_number, tok.col_number);
}

struct Eof {
    uint_fast32_t line_number;
    uint_fast32_t col_number;
};

struct Identifier {
    uint_fast32_t line_number;
    uint_fast32_t col_number;
    std::string lexeme;
};

// struct Plus {
//     uint_fast32_t line_number;
//     uint_fast32_t col_number;
// };

// struct Minus {
//     uint_fast32_t line_number;
//     uint_fast32_t col_number;
// };

// struct Dot {
//     uint_fast32_t line_number;
//     uint_fast32_t col_number;
// };

// struct Quote {
//     uint_fast32_t line_number;
//     uint_fast32_t col_number;
//     std::string quote;
// };

// struct Quasiquote {
//     uint_fast32_t line_number;
//     uint_fast32_t col_number;
//     std::string lexeme;
// };

// struct String {
//     uint_fast32_t line_number;
//     uint_fast32_t col_number;
//     std::string str;
// };

// struct True {
//     uint_fast32_t line_number;
//     uint_fast32_t col_number;
// };

// struct False {
//     uint_fast32_t line_number;
//     uint_fast32_t col_number;
// };

struct LParen {
    uint_fast32_t line_number;
    uint_fast32_t col_number;
};

struct RParen {
    uint_fast32_t line_number;
    uint_fast32_t col_number;
};

// using Token = std::variant<Eof, Identifier, Plus, Minus, Dot, Quote, Quasiquote, String, True,
//                           False, LParen, RParen>;

using Token = std::variant<Eof, LParen, RParen, Identifier>;

}  // namespace token

template <>
struct std::formatter<token::Eof> : std::formatter<std::string> {
    auto format(const token::Eof& tok, format_context& ctx) const {
        return formatter<string>::format(token::format_tok(tok, "EOF"), ctx);
    }
};

template <>
struct std::formatter<token::Identifier> : std::formatter<std::string> {
    auto format(const token::Identifier& tok, format_context& ctx) const {
        return formatter<string>::format(token::format_tok(tok), ctx);
    }
};

// template <>
// struct std::formatter<token::Plus> : std::formatter<std::string> {
//     auto format(const token::Plus& tok, format_context& ctx) const {
//         return formatter<string>::format(std::format("['+', line {}]", tok.line_number), ctx);
//     }
// };

// template <>
// struct std::formatter<token::Minus> : std::formatter<std::string> {
//     auto format(const token::Minus& tok, format_context& ctx) const {
//         return formatter<string>::format(std::format("['-', line {}]", tok.line_number), ctx);
//     }
// };
// template <>
// struct std::formatter<token::Dot> : std::formatter<std::string> {
//     auto format(const token::Dot& tok, format_context& ctx) const {
//         return formatter<string>::format(std::format("['.', line {}]", tok.line_number), ctx);
//     }
// };

// template <>
// struct std::formatter<token::Quote> : std::formatter<std::string> {
//     auto format(const token::Quote& tok, format_context& ctx) const {
//         return formatter<string>::format(
//             std::format("[quote {}, line {}]", tok.quote, tok.line_number), ctx);
//     }
// };

// template <>
// struct std::formatter<token::Quasiquote> : std::formatter<std::string> {
//     auto format(const token::Quasiquote& tok, format_context& ctx) const {
//         return formatter<string>::format(std::format("[{}, line {}]", tok.lexeme,
//         tok.line_number),
//                                          ctx);
//     }
// };

// template <>
// struct std::formatter<token::String> : std::formatter<std::string> {
//     auto format(const token::String& tok, format_context& ctx) const {
//         return formatter<string>::format(std::format("[\"{}\", line {}]", tok.str,
//         tok.line_number),
//                                          ctx);
//     }
// };

template <>
struct std::formatter<token::LParen> : std::formatter<std::string> {
    auto format(const token::LParen& tok, format_context& ctx) const {
        return formatter<string>::format(token::format_tok(tok, "("), ctx);
    }
};

template <>
struct std::formatter<token::RParen> : std::formatter<std::string> {
    auto format(const token::RParen& tok, format_context& ctx) const {
        return formatter<string>::format(token::format_tok(tok, ")"), ctx);
    }
};

template <>
struct std::formatter<token::Token> : std::formatter<std::string> {
    auto format(const token::Token& tok, format_context& ctx) const {
        struct Visitor {
            format_context& ctx;  // NOLINT
            auto operator()(const token::Eof tok) {
                return std::formatter<token::Eof>{}.format(tok, ctx);
            }
            auto operator()(const token::LParen& tok) {
                return std::formatter<token::LParen>{}.format(tok, ctx);
            }
            auto operator()(const token::RParen& tok) {
                return std::formatter<token::RParen>{}.format(tok, ctx);
            }
            auto operator()(const token::Identifier& tok) {
                return std::formatter<token::Identifier>{}.format(tok, ctx);
            }
        };

        return std::visit(Visitor{ctx}, tok);
    }
};
