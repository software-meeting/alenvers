#include <filesystem>
#include <fstream>
#include <ios>
#include <ranges>
#include <string>

#include "lexer.hpp"

auto main() -> int {
    std::string testa{"()"};
    // auto stream = std::basic_ifstream<char>{std::filesystem::path{"test.txt"}};
    // stream >> std::noskipws;
    // auto testb = std::ranges::istream_view<char>{stream};

    for (const auto& it : testa | lexer::lex<std::string_view>) {
        std::println("{}", token::print(it));
    }

    // for (const auto& it : testb | lexer::lex<std::ranges::istream_view<char>>) {
    //     std::println("{}", token::print(it));
    // }
}
