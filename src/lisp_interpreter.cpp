#include "lexer.hpp"
#include <print>
#include <ranges>
#include <string>
#include <string_view>

auto main() -> int {
    std::string testa{"(display \"sussy baka\")\n"};
    auto stream = std::basic_ifstream<char>{std::filesystem::path{"test.txt"}};
    auto testb = std::ranges::istream_view<char>{stream};

    for (const auto & it : testa | lexer::lex<std::string_view>) {
	std::println("{}", token::print(it));
    }

    for (const auto & it : testb | lexer::lex<std::ranges::istream_view<char>>) {
	std::println("{}", token::print(it));
    }
}
