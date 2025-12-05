// lexer.hpp
// Range adaptor on any range that yields a char

#pragma once

#include <concepts>
#include <cstdint>
#include <iterator>
#include <optional>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>

#include "char_range.hpp"
#include "lexer_types.hpp"
#include "match_char.hpp"
#include "token.hpp"
#include "util.hpp"

namespace lexer {

// Lexer
template <char_range::char_range R>
    requires std::same_as<std::iter_value_t<std::ranges::iterator_t<R>>, char> &&
             std::input_iterator<std::ranges::iterator_t<R>>
class Lexer {
   private:
    using r_iter_type = std::ranges::iterator_t<R>;
    using r_end_type = std::ranges::sentinel_t<R>;
    mutable R m_src;

    class Iterator {
       private:
        r_iter_type m_it{};
        r_end_type m_end{};
        State m_state{};
        std::string m_current_lexeme{};
        uint_fast32_t m_line_number{1};
        uint_fast32_t m_col_number{1};
        bool m_had_error{false};
        bool m_potential_crlf{};  // set this flag to true when we parse a CR

        token::Token m_tok{};

        // Advance the state of the Lexer FSM
        // m_it should be manually incremented when needed
        // yields Eof when stream has ended
        auto advance_state() -> LexResult {
            if (m_it == m_end) return LexResult{.token{token::Eof{}}, .state{InitState{}}};

            auto event = *m_it;

            if (m_potential_crlf && event != '\n') {
                m_line_number++;
                m_col_number = 1;
            }

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
                        return LexResult{
                            .token{token::Identifier{.line_number = m_line_number,
                                                     .col_number = m_col_number,
                                                     .lexeme{std::move(m_current_lexeme)}}}};
                    },

                    [this, event](const ErrorState& state) -> LexResult {
                        if (match_char::is_delimiter(event)) {
                            std::println("{}", InvalidTokenError{.line_number = m_line_number,
                                                                 .col_number = m_col_number,
                                                                 .lexeme{std::move(m_current_lexeme)}});

                            return LexResult{.token{std::nullopt}, .state{InitState{}}};
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
            } else if (!m_potential_crlf) {
                m_col_number++;
            }
            
            m_potential_crlf = false;
            if (event == '\r') {
                m_potential_crlf = true;
            }

            return result;
        }

        // Yield the next token
        auto parse_token() -> token::Token {
            while (true) {
                auto result = advance_state();
                if (result.token) {
                    return *result.token;
                }
            }
        }

       public:
        Iterator(r_iter_type begin, r_end_type end)
            : m_it(std::move(begin)), m_end(std::move(end)) {
            ++(*this);
        }

        // Iterator boilerplate
        using difference_type = std::ptrdiff_t;
        using value_type = token::Token;
        using reference = const token::Token&;
        using iterator_category = std::input_iterator_tag;

        auto operator*() const -> reference { return m_tok; }

        auto operator++() -> Iterator& {
            m_tok = parse_token();
            return *this;
        }

        auto operator++(int) -> Iterator {
            auto old = *this;
            ++(*this);
            return old;
        }

        auto operator==(std::default_sentinel_t other) const -> bool {
            return std::holds_alternative<token::Eof>(m_tok);
        }
    };

    static_assert(std::input_iterator<Iterator>);

   public:
    template <typename T>
        requires std::same_as<std::remove_cvref_t<T>, R>
    Lexer(T src) : m_src{std::forward<T>(src)} {}

    [[nodiscard]] auto begin() const { return Iterator(std::begin(m_src), std::end(m_src)); }

    [[nodiscard]] auto end() const { return std::default_sentinel; }
};

template <char_range::char_range R>
struct LexAdaptor : std::ranges::range_adaptor_closure<LexAdaptor<R>> {
    auto operator()(R src) const { return Lexer<R>{src}; }
};

template <char_range::char_range R>
constexpr LexAdaptor<R> lex{};

static_assert(std::ranges::range<lexer::Lexer<std::string_view>>);
static_assert(char_range::char_range<char_range::CharStreamRange>);
static_assert(std::ranges::range<lexer::Lexer<char_range::CharStreamRange>>);

}  // namespace lexer
