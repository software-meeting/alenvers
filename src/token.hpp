#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <cstdint>
#include <format>
#include <sstream>
#include <string>

namespace token {

enum struct TokenType : uint8_t { LPAREN, RPAREN, IDENTIFIER, END };

struct Token {
    TokenType m_type{};
    std::string m_lexeme{};
    unsigned int m_line_number{};

    Token() = default;

    explicit Token(TokenType&& type, std::string&& lexeme, unsigned int line_number)
        : m_type(type), m_lexeme(std::move(lexeme)), m_line_number(line_number) {}

    explicit Token(TokenType&& type, char lexeme, unsigned int line_number)
        : m_type(type), m_lexeme(std::string{lexeme}), m_line_number(line_number) {}
};

} // namespace token

template <> struct std::formatter<token::Token> {
    template <class ParseContext> constexpr ParseContext::iterator parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template <class FmtContext>
    auto format(const token::Token& tok, FmtContext& ctx) const -> FmtContext::iterator {
        std::ostringstream out;
        std::string type_str{};
        switch (tok.m_type) {
        case token::TokenType::LPAREN:
            type_str = "LPAREN";
            break;
        case token::TokenType::RPAREN:
            type_str = "RPAREN";
            break;
        case token::TokenType::IDENTIFIER:
            type_str = "IDENTIFIER";
            break;
        case token::TokenType::END:
            type_str = "END";
            break;
        }

        out << std::format("[{}, {}, line: {}]", type_str, tok.m_lexeme, tok.m_line_number);
        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};

#endif
