#pragma once

#include <cstdint>
#include <expected>
#include <format>
#include <optional>
#include <tuple>
#include <utility>
#include <variant>

#include "token.hpp"

namespace lexer_automaton {

struct Context {
    uint_fast32_t line_number{1};
    uint_fast32_t col_number{0};
    std::string lexeme{};
};

struct InvalidTokenError {
    uint_fast32_t line_number{};
    uint_fast32_t col_number{};
    std::string lexeme{};
};

using LexError = std::variant<InvalidTokenError>;

struct BaseState {};

struct InitState : public BaseState {};

struct ErrorState : BaseState {};

using State = std::variant<InitState, ErrorState>;
using LexResult = std::pair<std::optional<std::expected<token::Token, LexError>>, State>;

inline auto is_whitespace(const char c) -> bool { return c == ' ' || c == '\t' || c == '\n'; }

inline auto is_delimiter(const char c) -> bool { return is_whitespace(c) || c == ')' || c == '('; }

struct TransitionTable {
    auto operator()(InitState& state, Context& ctx, const char event) -> LexResult {
        switch (event) {
            case '(': {
                token::LParen tok{.line_number = ctx.line_number, .col_number = ctx.col_number};
                return {std::expected<token::Token, LexError>(tok), InitState{}};
            }

            case ')': {
                token::RParen tok{.line_number = ctx.line_number, .col_number = ctx.col_number};
                return {std::expected<token::Token, LexError>(tok), InitState{}};
            }

            default:
                return {std::nullopt, ErrorState{}};
        }
    }

    auto operator()(ErrorState& state, Context& ctx, const char event) -> LexResult {
        // Sync on delimiter
        if (is_delimiter(event)) {
            std::expected<token::Token, LexError> err =
                std::unexpected(InvalidTokenError{.line_number = ctx.line_number,
                                                  .col_number = ctx.col_number,
                                                  .lexeme = ctx.lexeme});
            return {std::make_optional(err), InitState{}};
        }
        ctx.lexeme += event;
        return {std::nullopt, ErrorState{}};
    }
};

class LexerAutomaton {
   private:
    State m_state{InitState{}};
    TransitionTable m_transitions{};
    std::optional<std::expected<token::Token, LexError>> m_tok{};
    Context m_ctx{};

   public:
    void process(const char event) {
        m_ctx.col_number++;
        if (event == '\n') {
            m_ctx.col_number = 0;
            m_ctx.line_number++;
        }

        std::tie(m_tok, m_state) =
            std::visit([&](auto& s) { return m_transitions(s, m_ctx, event); }, m_state);
    }

    auto get_token() -> std::optional<std::expected<token::Token, LexError>> { return m_tok; }
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

namespace lexer_automaton {
template <class... Ts>
struct overloads : Ts... {
    using Ts::operator()...;
};

auto print_lex_error(const LexError& err) -> std::string {
    auto visitor = overloads{[](const InvalidTokenError& err) { return std::format("{}", err); }};
    return std::visit(visitor, err);
}
}  // namespace lexer_automaton
