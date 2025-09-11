#include "token.hpp"
#include <cctype>
namespace token {

/// Whether byte c is a valid scheme identifier
bool is_identifier(char c) {
    switch (c) {
    case '!':
    case '$':
    case '%':
    case '&':
    case '*':
    case '+':
    case '-':
    case '.':
    case '/':
    case ':':
    case '<':
    case '=':
    case '>':
    case '?':
    case '@':
    case '^':
    case '_':
    case '~':
        return true;
    default:
        return std::isalnum(c);
    }
}

Token::Token(TokenType type) : m_type{type} {};

}; // namespace token
