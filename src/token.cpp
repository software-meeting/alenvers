#include "token.hpp"
#include <cctype>
#include <print>
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

Token::Token(TokenType&& type, std::string&& lexeme, unsigned int line_number)
    : m_type{type}, m_lexeme{lexeme}, m_line_number(line_number) {};

Token::Token(TokenType&& type, char lexeme, unsigned int line_number)
    : m_type{type}, m_lexeme{std::string{lexeme}}, m_line_number{line_number} {};

}; // namespace token
