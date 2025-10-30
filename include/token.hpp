#pragma once

#include <cstdint>
#include <format>
#include <string>
#include <variant>

#include "visitor.hpp"

namespace token {

struct Eof {};

// struct Identifier {
//     uint_fast32_t line_number;
//     uint_fast32_t col_number;
//     std::string lexeme;
// };

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

    using Token = std::variant<Eof, LParen, RParen>;

}  // namespace token

template <>
struct std::formatter<token::Eof> : std::formatter<std::string> {
    auto format(const token::Eof& tok, format_context& ctx) const {
        return formatter<string>::format("[EOF]", ctx);
    }
};

// template <>
// struct std::formatter<token::Identifier> : std::formatter<std::string> {
//     auto format(const token::Identifier& tok, format_context& ctx) const {
//         return formatter<string>::format(
//             std::format("['{}', line {}]", tok.lexeme, tok.line_number), ctx);
//     }
// };

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
        return formatter<string>::format(std::format("['(', line {}]", tok.line_number), ctx);
    }
};

template <>
struct std::formatter<token::RParen> : std::formatter<std::string> {
    auto format(const token::RParen& tok, format_context& ctx) const {
        return formatter<string>::format(std::format("[')', line {}]", tok.line_number), ctx);
    }
};

namespace token {
auto print(const Token& token) -> std::string {
    auto visitor = Visitor{
        [](const Eof& token) -> std::string { return std::format("{}", token); },
        // [](const Identifier& token) -> std::string { return std::format("{}", token); },
        [](const LParen& token) -> std::string { return std::format("{}", token); },
        [](const RParen& token) -> std::string { return std::format("{}", token); },
    };
    return std::visit(visitor, token);
}

};  // namespace token
