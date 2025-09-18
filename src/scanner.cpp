#include "scanner.hpp"

#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "token.hpp"

namespace scanner {

Scanner::Scanner(std::basic_ifstream<char>&& src) : m_src{std::move(src)} {};

// Check for:
// Braces
// Identifiers (no literals)
// Arithmetic operations

std::expected<Scanner, std::string> Scanner::create(std::string_view path) {
    std::filesystem::path fs_path{path};
    std::basic_ifstream<char> file{fs_path, std::ios::binary};
    if (!file.is_open())
        return std::unexpected("Failed to open file: " + fs_path.string());

    return Scanner{std::move(file)};
}

void Scanner::print_tokens() {
    for (auto t : scan().value()) {
	t.print();
    }
}

std::expected<std::vector<token::Token>, std::string> Scanner::scan() {
    if (m_src.eof())
        return std::unexpected("Input file is empty.");

    std::vector<token::Token> tokens{};
    int paren{0}; // keep track of parentheses

    unsigned int line_number{1};
    auto src_view = std::views::istream<char>(m_src);
    for (auto it = src_view.begin(); it != src_view.end();) {
        switch (*it) {
        case '\n':
            line_number++;
            it++;
            break;

	    
        case '\'': {
	    // QUOTE
	    // TODO: This is disgusting
            it++;
            std::string lexeme{};
            if (*it == '(') {
                lexeme += '(';
		paren++;
                while (*(++it) != ')') {

                    if (*it == '\n') {
                        return std::unexpected(
                            std::format("[Line {}]: Missing closing parenthesis.", line_number));
                    }
                    lexeme += *it;
                }
                lexeme += ')';
		paren--;
                tokens.push_back(
                    token::Token{token::TokenType::QUOTE, std::move(lexeme), line_number});
                it++;
                break;
            }
            while (!token::is_delimiter(*(++it))) {
                lexeme += *it;
            }
            tokens.push_back(token::Token{token::TokenType::QUOTE, std::move(lexeme), line_number});
            it++;
            break;
        }
        case '(':
            paren++;
            tokens.push_back(token::Token{token::TokenType::LPAREN, *it, line_number});
            it++;
            break;

        case ')':
            if (paren == 0)
                return std::unexpected(
                    std::format("[Line {}]: Extraneous closing parenthesis.", line_number));
            paren--;
            tokens.push_back(token::Token{token::TokenType::RPAREN, *it, line_number});
            it++;
            break;

        default: {
	    std::string lexeme{};
	    while (!token::is_delimiter(*it)) {
		lexeme += *it;
		it++;
            }
	    tokens.push_back(token::Token{token::TokenType::IDENTIFIER, std::move(lexeme), line_number});
            it++;
	    break;            
	}
        }
    }

    if (paren > 0) {
        return std::unexpected(std::format("[Line {}]: Missing closing parenthesis.", line_number));
    }
    return tokens;
}

void Scanner::print_file() {
    std::string c{std::istreambuf_iterator<char>{m_src}, std::istreambuf_iterator<char>{}};
    std::print("{}", c);
}

} // namespace scanner
