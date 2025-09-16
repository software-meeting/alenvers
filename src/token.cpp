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

Token::Token(TokenType&& type, std::u32string&& lexeme, unsigned int line_number)
    : m_type{type}, m_lexeme{lexeme}, m_line_number(line_number) {};

}; // namespace token
