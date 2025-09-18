#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <expected>
#include <fstream>
#include <string_view>
#include <vector>

#include "token.hpp"

namespace scanner {
class Scanner {
  public:
    static std::expected<Scanner, std::string> create(std::string_view path);

    std::expected<std::vector<token::Token>, std::string> scan();

    void print_tokens();    

    void print_file();

  private:
    std::basic_ifstream<char> m_src;

    explicit Scanner(std::basic_ifstream<char>&& src);

    std::expected<token::Token, std::string> scan_token();
};

} // namespace scanner

#endif
