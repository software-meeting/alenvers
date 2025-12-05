#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <string>

#include "lexer.hpp"

auto main() -> int {
    try {
        std::string testa {
            "(define (fib a)\n\
               (define (fib-iter a b n)\n\
                 (if (= n 0)\n\
                  b\n\
                  (fib-iter b (+ a b) (- n 1))\n\
                 )\n\
               )\n\
               (fib 1 1 a))"};

        auto file = std::basic_ifstream<char>{std::filesystem::path{"test.txt"}};
        auto testb = std::ranges::subrange{std::istreambuf_iterator<char>{file},
                                                 std::istreambuf_iterator<char>{}};

        for (const auto& it : testa | lexer::lex) {
            if (it) std::println("{}", *it);
            else std::println("{}", it.error());
        }

        std::println("\n=== File test... ===\n");
        
        for (const auto& it : testb | lexer::lex) {
            if (it) std::println("{}", *it);
            else std::println("{}", it.error());
        }

    } catch (...) {
        std::cerr << "Something went very wrong, please contact the developer.\n";
        return 1;
    }
}
