#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

namespace token {
/// returns if c is a valid scheme identifier
/// also returns true if the character is a ., so be careful with lists
bool is_identifier(char c);

bool is_delimiter(char c);

enum TokenType { LPAREN, RPAREN, IDENTIFIER};

struct Token {
    TokenType m_type;
    std::string m_lexeme;

    Token(TokenType type);
    Token(TokenType type, std::string m_lexeme);
};

} // namespace token

#endif
