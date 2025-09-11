#ifndef TOKEN_HPP
#define TOKEN_HPP

namespace token {
/// returns if c is a valid scheme identifier
/// also returns true if the character is a ., so be careful with lists
bool is_identifier(char c);

enum TokenType { LPAREN, RPAREN, PIPE };

struct Token {
    TokenType m_type;
    Token(TokenType type);
}

} // namespace token

#endif
