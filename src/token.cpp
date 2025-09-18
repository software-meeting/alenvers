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

std::string print_token(TokenType type) {
    switch (type) {

    case LPAREN:
        return std::string{"LPAREN"};
    case RPAREN:
        return std::string{"RPAREN"};
    case IDENTIFIER:
        return std::string{"IDENTIFIER"};
    case QUOTE:
        return std::string{"QUOTE"};
    }
}

Token::Token(TokenType&& type, std::string&& lexeme, unsigned int line_number)
    : m_type{type}, m_lexeme{lexeme}, m_line_number(line_number) {};

Token::Token(TokenType&& type, char lexeme, unsigned int line_number)
    : m_type{type}, m_lexeme{std::string{lexeme}}, m_line_number{line_number} {};

void Token::print() {
    std::print("[");
    std::print("{},", m_lexeme);
    std::print("{}", print_token(m_type));
    std::print("]");

}

}; // namespace token
