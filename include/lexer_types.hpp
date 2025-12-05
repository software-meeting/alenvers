// lexer_types.hpp
// types for use in the lexer mechanism

#pragma once

#include <cstdint>
#include <expected>
#include <optional>
#include <string>
#include <variant>

#include "token.hpp"

namespace lexer {

struct InvalidTokenError {
    uint_fast32_t line_number{};
    uint_fast32_t col_number{};
    std::string lexeme{};
};

using LexError = std::variant<InvalidTokenError>;

struct InitState {};
struct IdentifierState {};
struct ErrorState {};

using State = std::variant<InitState, IdentifierState, ErrorState>;

struct LexResult {
    std::optional<std::expected<token::Token, LexError>> token;
    State state;
};

}  // namespace lexer

template <>
struct std::formatter<lexer::InvalidTokenError> : std::formatter<std::string> {
    auto format(const lexer::InvalidTokenError& err, format_context& ctx) const {
        return formatter<string>::format(
            std::format("Error [line: {}, column: {}]: Unknown token '{}'.", err.line_number,
                        err.col_number, err.lexeme),
            ctx);
    }
};

template <>
struct std::formatter<lexer::LexError> : std::formatter<std::string> {
    auto format(const lexer::LexError& err, format_context& ctx) const {
        struct Visitor {
            format_context& ctx;  // NOLINT
            auto operator()(const lexer::InvalidTokenError& err) {
                return std::formatter<lexer::InvalidTokenError>{}.format(err, ctx);
            }
        };
        return std::visit(Visitor{ctx}, err);
    }
};
