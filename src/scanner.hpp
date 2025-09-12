#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <expected>
#include <fstream>
#include <string_view>


namespace scanner {
    class Scanner {
      public:
	static std::expected<Scanner, std::string> create(std::string_view path);
	void print_file();

      private:
	std::ifstream m_src;
	explicit Scanner(std::ifstream&& src);
    };
}

#endif
