#include "scanner.hpp"

#include <expected>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <print>
#include <string_view>
#include <utility>

#include "sad.hpp"

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

void Scanner::print_file() {
    std::u32string contents{std::istreambuf_iterator<char32_t>(m_src), std::istreambuf_iterator<char32_t>{}};

    std::print("{}", sad::conv::utf32_to_utf8.to_bytes(contents));
}

} // namespace scanner
