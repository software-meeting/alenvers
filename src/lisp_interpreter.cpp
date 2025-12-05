#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>

#include "lexer.hpp"

auto main() -> int {
    try {
        std::string testa{"()"};

        auto file = std::basic_ifstream<char>{std::filesystem::path{"test.txt"}};
        auto testb = char_range::CharStreamRange{std::istreambuf_iterator<char>{file},
                                                 std::istreambuf_iterator<char>{}};

        for (const auto& it : testa | lexer::lex<std::string_view>) {
            std::println("{}", it);
        }

        for (const auto& it : testb | lexer::lex<char_range::CharStreamRange>) {
            std::println("{}", it);
        }
        
    } catch (...) {
        std::cerr << "Something went very wrong, please contact the developer.\n";
	return 1;        
    }
}
