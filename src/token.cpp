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

bool is_delimiter(char c) {
    switch (c) {
    case ' ':
    case '|':
    case '(':
    case ')':
    case '"':
    case ';':
    case EOF:
        return true;
    default:
        return false;
    }
}

Token::Token(TokenType type) : m_type{type}, m_lexeme{""} {};

Token::Token(TokenType type, std::string lexeme) : m_type{type}, m_lexeme{std::move(lexeme)} {};

}; // namespace token
