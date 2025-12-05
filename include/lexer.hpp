// lexer.hpp
// Range adaptor on any range that yields a char

#pragma once

#include <concepts>
#include <cstdint>
#include <expected>
#include <iterator>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <variant>

#include "lexer_types.hpp"
#include "match_char.hpp"
#include "token.hpp"
#include "util.hpp"

namespace lexer {

// Lexer
template <std::ranges::input_range R>
    requires std::same_as<std::iter_value_t<std::ranges::iterator_t<R>>, char>
class Lexer : public std::ranges::view_interface<Lexer<R>> {
   private:
    R m_src;

   public:
    class Iterator {
       private:
        using r_iter_type = std::ranges::iterator_t<R>;
        using r_end_type = std::ranges::sentinel_t<R>;

        r_iter_type m_it{};
        r_end_type m_end{};
        bool m_at_end{false};

        State m_state{};
        std::string m_current_lexeme{};
        uint_fast32_t m_line_number{1};
        uint_fast32_t m_col_number{0};
        bool m_had_error{false};

        std::expected<token::Token, LexError> m_tok{};

        // Advance the state of the Lexer FSM
        // m_it should be manually incremented when needed
        // yields Eof when stream has ended
        auto advance_state() -> LexResult {
            m_col_number++;
            
            if (m_it == m_end) {
                if (m_tok && std::holds_alternative<token::Eof>(*m_tok)) {
                    m_at_end = true;
                    return LexResult{.token{token::Eof{.line_number = m_line_number,
                                                       .col_number = m_col_number}},
                                     .state{InitState{}}};
                }
                return LexResult{
                    .token{token::Eof{.line_number = m_line_number, .col_number = m_col_number}},
                    .state{InitState{}}};
            }

            auto event = *m_it;

            auto result = std::visit(
                util::overloads{
                    [this, event](const InitState& state) -> LexResult {
                        if (std::iswspace(event)) {
                            m_it++;  // NOLINT
                            return LexResult{.token{std::nullopt}, .state{InitState{}}};
                        }

                        if (match_char::is_initial(event)) {
                            m_it++;  // NOLINT
                            m_current_lexeme = event;
                            return LexResult{.token{std::nullopt}, .state{IdentifierState{}}};
                        }

                        switch (event) {
                            case '(': {
                                token::LParen tok{.line_number = m_line_number,
                                                  .col_number = m_col_number};
                                m_it++;  // NOLINT
                                return LexResult{.token{tok}, .state{InitState{}}};
                            }

                            case ')': {
                                token::RParen tok{.line_number = m_line_number,
                                                  .col_number = m_col_number};
                                m_it++;  // NOLINT
                                return LexResult{.token{tok}, .state{InitState{}}};
                            }

                            default:
                                // enter error state and try to resynchronise
                                m_it++;  // NOLINT
                                m_current_lexeme = event;
                                return LexResult{.token{std::nullopt}, .state{ErrorState{}}};
                        }
                    },

                    [this, event](const IdentifierState& state) -> LexResult {
                        if (match_char::is_subsequent(event)) {
                            m_it++;  // NOLINT
                            m_current_lexeme += event;
                            return LexResult{.token{std::nullopt}, .state{IdentifierState{}}};
                        }
                        auto result = LexResult{
                            .token{token::Identifier{.line_number = m_line_number,
                                                     .col_number = m_col_number - m_current_lexeme.length(),
                                                     .lexeme{std::move(m_current_lexeme)}}}};
                        m_current_lexeme = {};
                        return result;
                    },

                    [this, event](const ErrorState& state) -> LexResult {
                        if (match_char::is_delimiter(event)) {
                            auto result = LexResult{.token{std::unexpected(InvalidTokenError{
                                                        .line_number = m_line_number,
                                                        .col_number = m_col_number,
                                                        .lexeme{std::move(m_current_lexeme)}})},
                                                    .state{InitState{}}};
                            m_current_lexeme = {};
                            return result;
                        }

                        m_current_lexeme += event;
                        m_it++;  // NOLINT
                        return LexResult{.token{std::nullopt}, .state{ErrorState{}}};
                    },

                },
                m_state);

            m_state = result.state;

            if (event == '\n') {
                m_col_number = 1;
                m_line_number++;
            }

            return result;
        }

        // Yield the next token
        auto parse_token() -> std::expected<token::Token, LexError> {
            while (true) {
                auto result = advance_state();
                if (result.token) {
                    return *result.token;
                }
            }
        }

       public:
        Iterator(r_iter_type begin, r_end_type end)
            : m_it{std::move(begin)}, m_end{std::move(end)}, m_tok{parse_token()} {}

        // Iterator boilerplate
        using difference_type = std::ptrdiff_t;
        using value_type = std::expected<token::Token, LexError>;
        using iterator_concept = std::input_iterator_tag;

        auto operator*() const -> const std::expected<token::Token, LexError>& { return m_tok; }

        auto operator++() -> Iterator& {
            m_tok = parse_token();
            return *this;
        }

        void operator++(int) { ++(*this); }

        auto operator==(std::default_sentinel_t other) const -> bool { return m_at_end; }
    };

    static_assert(std::input_iterator<Iterator>);

    Lexer() = default;
    explicit Lexer(R src) : m_src{std::move(src)} {}

    auto begin() { return Iterator{std::ranges::begin(m_src), std::ranges::end(m_src)}; }
    auto end() { return std::default_sentinel; }
};

struct LexAdaptorClosure {
    template <std::ranges::viewable_range R>
    auto operator()(R&& r) const {
        auto normalised_view = util::newline_normaliser_adapter(std::forward<R>(r));
        return Lexer<decltype(normalised_view)>{std::move(normalised_view)};
    }

    template <std::ranges::viewable_range R>
    friend auto operator|(R&& r, LexAdaptorClosure closure) {
        return closure(std::forward<R>(r));
    }
};

constexpr LexAdaptorClosure lex;

static_assert(std::ranges::range<lexer::Lexer<std::string_view>>);

}  // namespace lexer
