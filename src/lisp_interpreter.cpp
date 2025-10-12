#include "lexer.hpp"
#include <print>
#include <ranges>
#include <string>
#include <string_view>

auto main() -> int {
    std::string testa{"(display \"sussy baka\")\n"};
    auto stream = std::basic_ifstream<char>{std::filesystem::path{"test.txt"}};
    auto testb = std::ranges::istream_view<char>{stream};

    std::println("{}", testa | lexer::lex<std::string_view>);
    std::println("{}", testb | lexer::lex<std::ranges::istream_view<char>>);

}
