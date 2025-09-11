#include <expected>
#include <iostream>
#include <print>
#include <string>

#include "scanner.hpp"

int main() {
    auto res = scanner::Scanner::create("test.txt")
        .and_then([](scanner::Scanner&& s) {
            s.print_file();
            return std::expected<void, std::string>{};
        })
        .or_else([](const std::string& err) {
            std::cerr << "Failed: " << err << '\n';
            return std::expected<void, std::string>{std::unexpected(err)};
        });

    return !res.has_value();
}
