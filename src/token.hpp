#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <format>
#include <sstream>
#include <string>

namespace token {
/// returns if c is a valid scheme identifier
/// also returns true if the character is a ., so be careful with lists
bool is_identifier(char c);

bool is_delimiter(char c);

enum TokenType { LPAREN, RPAREN, IDENTIFIER, QUOTE };

class Token {
  public:
    TokenType m_type;
    std::string m_lexeme;
    unsigned int m_line_number;

    explicit Token(TokenType&& type, std::string&& m_lexeme, unsigned int line_number);
    explicit Token(TokenType&& type, char m_lexeme, unsigned int line_number);
};

} // namespace token

template <> struct std::formatter<token::Token, char> {
    template <class ParseContext> constexpr ParseContext::iterator parse(ParseContext& ctx) {
        auto it = ctx.begin();
        return it;
    }

    template <class FmtContext> FmtContext::iterator format(token::Token t, FmtContext& ctx) const {
        std::ostringstream out;
        auto type_string = std::string{};
        switch (t.m_type) {
        case token::LPAREN:
            type_string = "LPAREN";
            break;
        case token::RPAREN:
            type_string = "RPAREN";
            break;
        case token::IDENTIFIER:
            type_string = "IDENTIFIER";
            break;
        case token::QUOTE:
            type_string = "QUOTE";
            break;
        }
        out << std::format("( token: {}, {} )", std::move(type_string), t.m_lexeme);
        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};

#endif
