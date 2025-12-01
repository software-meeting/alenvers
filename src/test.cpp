#include <gtest/gtest.h>

#include <variant>

#include "lexer.hpp"
#include "token.hpp"

TEST(lexer_test, parentheses_pair) {
    std::string s{"()"};
    auto stream = s | lexer::lex<std::string_view>;
    auto it = stream.begin();
    EXPECT_TRUE(std::holds_alternative<token::LParen>(*it));
    it++;
    EXPECT_TRUE(std::holds_alternative<token::RParen>(*it));
    it++;
    EXPECT_TRUE(it == stream.end());
}
