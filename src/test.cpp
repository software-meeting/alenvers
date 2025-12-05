#include <gtest/gtest.h>

#include <variant>

#include "lexer.hpp"
#include "token.hpp"

TEST(lexer_test, parentheses_pair) {
    std::string s{"()"};
    auto stream = s | lexer::lex;
    auto it = stream.begin();
    
    EXPECT_TRUE(*it);
    EXPECT_TRUE(std::holds_alternative<token::LParen>(**it));
    it++;
    
    EXPECT_TRUE(*it);    
    EXPECT_TRUE(std::holds_alternative<token::RParen>(**it));
    it++;
    
    EXPECT_TRUE(*it);
    EXPECT_TRUE(std::holds_alternative<token::Eof>(**it));    
    EXPECT_TRUE(it == stream.end());
}

TEST(lexer_test, simple_identifiers) {
    std::string s{"(among us sussy)"};
    auto stream = s | lexer::lex;
    auto it = stream.begin();

    EXPECT_TRUE(*it);
    EXPECT_TRUE(std::holds_alternative<token::LParen>(**it));
    it++;

    EXPECT_TRUE(*it);    
    EXPECT_TRUE(std::holds_alternative<token::Identifier>(**it));
    EXPECT_STREQ(std::get<token::Identifier>(**it).lexeme.c_str(), "among");
    it++;

    EXPECT_TRUE(*it);    
    EXPECT_TRUE(std::holds_alternative<token::Identifier>(**it));
    EXPECT_STREQ(std::get<token::Identifier>(**it).lexeme.c_str(), "us");
    it++;

    EXPECT_TRUE(*it);    
    EXPECT_TRUE(std::holds_alternative<token::Identifier>(**it));
    EXPECT_STREQ(std::get<token::Identifier>(**it).lexeme.c_str(), "sussy");
    it++;

    EXPECT_TRUE(*it);
    EXPECT_TRUE(std::holds_alternative<token::RParen>(**it));
    it++;

    EXPECT_TRUE(*it);
    EXPECT_TRUE(std::holds_alternative<token::Eof>(**it));
    EXPECT_TRUE(it == stream.end());    
}


TEST(lexer_test, line_whitespace) {
    std::string s{"(\r\namong\r us\r\n sussy\n)\n"};
    auto stream = s | lexer::lex;
    auto it = stream.begin();

    EXPECT_TRUE(std::holds_alternative<token::LParen>(**it));
    it++;
    
    EXPECT_TRUE(std::holds_alternative<token::Identifier>(**it));
    EXPECT_STREQ(std::get<token::Identifier>(**it).lexeme.c_str(), "among");
    it++;

    EXPECT_TRUE(std::holds_alternative<token::Identifier>(**it));
    EXPECT_STREQ(std::get<token::Identifier>(**it).lexeme.c_str(), "us");
    it++;

    EXPECT_TRUE(std::holds_alternative<token::Identifier>(**it));
    EXPECT_STREQ(std::get<token::Identifier>(**it).lexeme.c_str(), "sussy");
    it++;    
    
    EXPECT_TRUE(std::holds_alternative<token::RParen>(**it));
    it++;

    EXPECT_TRUE(std::holds_alternative<token::Eof>(**it));
    EXPECT_TRUE(it == stream.end());    
}
