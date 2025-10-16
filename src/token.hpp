#pragma once

#include <format>
#include <string>
#include <variant>

namespace token {

template <class... Ts> struct overloads : Ts... {
    using Ts::operator()...;
};

struct Identifier {
    unsigned int m_line_number;
    std::string m_lexeme;
};

struct LParen {
    unsigned int m_line_number;
};

struct RParen {
    unsigned int m_line_number;
};

using Token = std::variant<Identifier, LParen, RParen>;

} // namespace token

template <> struct std::formatter<token::Identifier> : std::formatter<std::string> {
    auto format(const token::Identifier& tok, format_context& ctx) const {
        return formatter<string>::format(
            std::format("['{}', line {}]", tok.m_lexeme, tok.m_line_number), ctx);
    }
};

template <> struct std::formatter<token::LParen> : std::formatter<std::string> {
    auto format(const token::LParen& tok, format_context& ctx) const {
        return formatter<string>::format(std::format("['(', line {}]", tok.m_line_number), ctx);
    }
};

template <> struct std::formatter<token::RParen> : std::formatter<std::string> {
    auto format(const token::RParen& tok, format_context& ctx) const {
        return formatter<string>::format(std::format("[')', line {}]", tok.m_line_number), ctx);
    }
};

namespace token {
auto print(const Token& token) -> std::string {
    auto visitor = overloads{
        [](const Identifier& token) -> std::string { return std::format("{}", token); },
        [](const LParen& token) -> std::string { return std::format("{}", token); },
        [](const RParen& token) -> std::string { return std::format("{}", token); },

    };
    return std::visit(visitor, token);
}

}; // namespace token
