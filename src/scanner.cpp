#include "scanner.hpp"

#include <cstdio>
#include <expected>
#include <filesystem>
#include <print>
#include <string_view>

#include "fileptr.hpp"
#include "token.hpp"

namespace scanner {

using TokenType = token::TokenType;

Scanner::Scanner(fileptr::FilePtr src) : m_src(std::move(src)) {};

std::expected<Scanner, std::string> Scanner::create(std::string_view path) {
    std::filesystem::path fs_path{path};
    std::FILE* file = std::fopen(fs_path.c_str(), "r");
    if (!file) {
        return std::unexpected("Failed to open file: " + fs_path.string());
    }

    return Scanner(fileptr::FilePtr{file});
}

std::expected<std::vector<token::Token>, std::string> Scanner::scan() {
    std::vector<token::Token> token_list{};

    switch (std::fgetc(m_src.get())) {
    case '(':
	m_paren_stack.push('(');
	token_list.push_back(token::Token(TokenType::LPAREN));
	break;
    case ')':
	if (m_paren_stack.empty()) return std::unexpected("Missing closing parenthesis.");
	token_list.push_back(token::Token(TokenType::RPAREN));
	break;
    default:
    }
}

void Scanner::print_file() {
    char buf[256];
    for (;;) {
        int nread = std::fread(buf, sizeof(char), 256, m_src.get());
        if (nread == 0)
            break;
        std::print("{}", std::string_view(buf, nread));
        if (nread < 256)
            break;
    }
}

} // namespace scanner
