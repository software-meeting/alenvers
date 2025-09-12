#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <cstdint>
#include <expected>
#include <fstream>
#include <string_view>
#include <vector>

#include "token.hpp"

namespace scanner {
class Scanner {
  public:
    static std::expected<Scanner, std::string> create(std::string_view path);
    void print_file();
    std::expected<std::vector<token::Token>, std::string> scan();

  private:
    std::basic_ifstream<char32_t> m_src;
    explicit Scanner(std::basic_ifstream<char32_t>&& src);
    std::uint32_t get_character(std::vector<char>& char_stream);
    int parse_to_uint32(std::uint32_t* dest);
};
} // namespace scanner

#endif
