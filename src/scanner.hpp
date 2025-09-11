#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <expected>
#include <stack>
#include <string_view>
#include <vector>

#include "fileptr.hpp"
#include "token.hpp"

namespace scanner {
    class Scanner {
      public:
	static std::expected<Scanner, std::string> create(std::string_view path);
	void print_file();

        std::expected<std::vector<token::Token>, std::string> scan();

      private:
	fileptr::FilePtr m_src;
	std::stack<char> m_paren_stack;

	explicit Scanner(fileptr::FilePtr src);
    };
}

#endif
