#include <expected>
#include <print>
#include <string_view>

#include "scanner.hpp"

int main() {
    auto res = scanner::Scanner::create("test.txt");
    if (!res) {
	std::print(stderr, "{}", res.error());
	return 1;
    }
    std::println("No errors!");
    return 0;
}
