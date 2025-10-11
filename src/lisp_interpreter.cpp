#include "lexer.hpp"
#include <print>
#include <string>

auto main() -> int {
    std::string test{"(display \"sussy baka\")"};
    for (int i : std::ranges::iota_view(0, 5)) {
        std::print("{}", test | lexer::lex);
    }
}
