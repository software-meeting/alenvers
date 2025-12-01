#pragma once

#include <cstdint>
#include <cwctype>
#include <expected>
#include <format>
#include <optional>
#include <variant>

#include "token.hpp"

namespace lexer_automaton {

// FSA infastructure

struct InvalidTokenError {
    uint_fast32_t line_number{};
    uint_fast32_t col_number{};
    std::string lexeme{};
};

using LexError = std::variant<InvalidTokenError>;

struct BaseState {};

struct InitState : BaseState {};
// struct ErrorState : BaseState {};
// struct IdentifierState : BaseState {};

using State = std::variant<InitState>;

struct LexResultData {
    std::optional<token::Token> token;
    State state;
};

using LexResult = std::expected<LexResultData, LexError>;

// matching characters

template <const char... match>
auto match_char(const char c) -> bool {
    return ((c == match) || ...);
}

auto is_delimiter(const char c) -> bool {
    return std::iswspace(c) || match_char<'(', ')', '"', ';'>(c);
}

// FSA

struct TransitionTable {
    std::string_view current_lexeme_prefix;
    char event;
    uint_fast32_t line_number;
    uint_fast32_t col_number;

    auto operator()(const InitState& state) -> LexResult {
        if (std::iswspace(event))
            return LexResultData{.token{std::nullopt}, .state{InitState{}}};
                    
        switch (event) {
            case '(': {
                token::LParen tok{.line_number = line_number, .col_number = col_number};
                return LexResultData{.token{tok}, .state{InitState{}}};
            }

            case ')': {
                token::RParen tok{.line_number = line_number, .col_number = col_number};
                return LexResultData{.token{tok}, .state{InitState{}}};
            }

            default:
                return std::unexpected(
                    InvalidTokenError{.line_number = line_number,
                                      .col_number = col_number,
                                      .lexeme{std::string{current_lexeme_prefix}}});
        }
    }

    // auto operator()(IdentifierState& state, Context& ctx, const char event) -> LexResult {
    //     if (is_delimiter(event) || event == EOF) {
    //         ctx.should_increment = false;
    //         token::Identifier tok{
    //             .line_number = ctx.line_number, .col_number = ctx.col_number, .lexeme =
    //             ctx.lexeme};
    //         return {tok, InitState{}};
    //     }

    //     ctx.lexeme += event;
    //     return {std::nullopt, IdentifierState{}};
    // }

    // auto operator()(ErrorState& state, Context& ctx, const char event) -> LexResult {
    //     // Sync on delimiter
    //     if (is_delimiter(event) || event == EOF) {
    //         std::expected<token::Token, LexError> err =
    //             std::unexpected(InvalidTokenError{.line_number = ctx.line_number,
    //                                               .col_number = ctx.col_number,
    //                                               .lexeme = ctx.lexeme});
    //         return {std::make_optional(err), InitState{}};
    //     }

    //     ctx.lexeme += event;
    //     return {std::nullopt, ErrorState{}};
    // }
};
}  // namespace lexer_automaton

template <>
struct std::formatter<lexer_automaton::InvalidTokenError> : std::formatter<std::string> {
    auto format(const lexer_automaton::InvalidTokenError& err, format_context& ctx) const {
        return formatter<string>::format(
            std::format("Error [line: {}, column: {}]: Unknown token '{}'.", err.line_number,
                        err.col_number, err.lexeme),
            ctx);
    }
};

template <>
struct std::formatter<lexer_automaton::LexError> : std::formatter<std::string> {
    auto format(const lexer_automaton::LexError& err, format_context& ctx) const {
        struct Visitor {
            format_context& ctx;  // NOLINT
            auto operator()(const lexer_automaton::InvalidTokenError& err) {
                return std::formatter<lexer_automaton::InvalidTokenError>{}.format(err, ctx);
            }
        };
        return std::visit(Visitor{ctx}, err);
    }
};
