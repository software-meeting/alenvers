#include "lexer.hpp"
#include <print>
#include <string>

auto main() -> int {
    std::string test{"(display \"sussy baka\")"};
    lexer::Lexer<std::string> amongus(test);
    auto a = amongus.begin();
    for (int i : std::ranges::iota_view(0, 5)) {
        std::print("{}", (*a).m_lexeme);
    }
}
