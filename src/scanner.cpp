#include "scanner.hpp"

#include <expected>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <print>
#include <string_view>
#include <utility>

namespace scanner {

Scanner::Scanner(std::ifstream&& src) : m_src{std::move(src)} {};

// Check for:
// Braces
// Identifiers (no literals)
// Arithmetic operations

std::expected<Scanner, std::string> Scanner::create(std::string_view path) {
    std::filesystem::path fs_path{path};
    std::ifstream file{fs_path, std::ios::binary};
    if (!file.is_open())
        return std::unexpected("Failed to open file: " + fs_path.string());

    return Scanner{std::move(file)};
}

void Scanner::print_file() {
    std::string contents{std::istreambuf_iterator<char>(m_src), std::istreambuf_iterator<char>{}};

    std::print("{}", contents);
}

} // namespace scanner
