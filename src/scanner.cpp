#include "scanner.hpp"

#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <stack>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "char32_t_format.hpp"
#include "token.hpp"

namespace scanner {

Scanner::Scanner(std::basic_ifstream<char32_t>&& src) : m_src{std::move(src)} {};

// Check for:
// Braces
// Identifiers (no literals)
// Arithmetic operations

std::expected<Scanner, std::string> Scanner::create(std::string_view path) {
    std::filesystem::path fs_path{path};
    std::basic_ifstream<char32_t> file{fs_path, std::ios::binary};
    if (!file.is_open())
        return std::unexpected("Failed to open file: " + fs_path.string());

    return Scanner{std::move(file)};
}

std::expected<std::vector<token::Token>, std::string> Scanner::scan() {
    if (m_src.eof())
        return std::unexpected("Input file is empty.");

    std::vector<token::Token> tokens{};
    std::stack<char> paren{}; // keep track of parentheses

    unsigned int line_number{1};
    auto view = std::views::istream<char32_t>(m_src);
    for (auto it = view.begin(); it != view.end(); it++) {
        switch (*it) {
        case '\n':
            line_number++;
            break;

        case '(':
            tokens.push_back(
                token::Token{token::TokenType::LPAREN, std::u32string{*it}, line_number});
            break;

        case ')':
            if (paren.empty())
                return std::unexpected(
                    std::format("[Line {}]: Extraneous closing parenthesis.", line_number));
            paren.pop();
            tokens.push_back(
                token::Token{token::TokenType::RPAREN, std::u32string{*it}, line_number});
            break;
        default:
            return std::unexpected(
                std::format("[Line {}]: Unknown token {}", line_number,
                            char32_t_format::utf32_to_utf8(std::u32string{*it}).value()));
        }
    }
    return tokens;
}

void Scanner::print_file() {
    std::u32string c{std::istreambuf_iterator<char32_t>{m_src},
                     std::istreambuf_iterator<char32_t>{}};
    std::print("{}", char32_t_format::utf32_to_utf8(c).value());
}

} // namespace scanner
