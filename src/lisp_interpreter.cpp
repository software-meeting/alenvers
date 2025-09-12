#include <expected>
#include <print>

#include "scanner.hpp"

int main() {
    auto res = scanner::Scanner::create("test.txt");
    if (!res) {
	std::print(stderr, "{}", res.error());
	return 1;
    }
    res.value().print_file();
}
