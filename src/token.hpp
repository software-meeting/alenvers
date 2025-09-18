#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

namespace token {
/// returns if c is a valid scheme identifier
/// also returns true if the character is a ., so be careful with lists
bool is_identifier(char c);

bool is_delimiter(char c);

    enum TokenType { LPAREN, RPAREN, IDENTIFIER, QUOTE};

struct Token {
    TokenType m_type;
    std::string m_lexeme;
    unsigned int m_line_number;

    void print();

    explicit Token(TokenType&& type, std::string&& m_lexeme, unsigned int line_number);
    explicit Token(TokenType&& type, char m_lexeme, unsigned int line_number);
};

} // namespace token

#endif
