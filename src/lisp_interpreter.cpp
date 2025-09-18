#include <expected>
#include <print>
#include <string_view>

#include "scanner.hpp"

int main() {
    auto res = scanner::Scanner::create("test.txt");
    res.value().print_tokens();
}
