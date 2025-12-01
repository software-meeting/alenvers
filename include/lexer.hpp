#pragma once

#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <expected>
#include <iterator>
#include <optional>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>

#include "char_range.hpp"
#include "lexer_automaton.hpp"
#include "token.hpp"

namespace lexer {
struct InvalidPathError {
    std::string filename;
};

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
        lexer_automaton::State m_state{};
        std::string m_current_lexeme{};
        uint_fast32_t m_line_number{1};
        uint_fast32_t m_col_number{1};

        token::Token m_tok{};

        auto parse_token() -> token::Token {
            while (m_it != m_end) {
                lexer_automaton::TransitionTable visitor{.current_lexeme_prefix{m_current_lexeme},
                                                         .event = *m_it,
                                                         .line_number = m_line_number,
                                                         .col_number = m_col_number};

                auto result = std::visit(visitor, m_state);
                if (!result.has_value()) {
                    std::println("amongus");
                    std::exit(EXIT_FAILURE);
                }

                m_state = result.value().state;
                if (*m_it == '\n') {
                    m_col_number = 1;
                    m_line_number++;
                } else {
                    m_col_number++;
                }

                if (result.value().token.has_value()) {
                    auto tok = result.value().token.value();
                    if (token::should_consume(tok)) m_it++;  // NOLINT
                    return tok;
                }

                m_it++;  // NOLINT
            }
            return token::Eof{};
        }

       public:
        Iterator(r_iter_type begin, r_end_type end)
            : m_it(std::move(begin)), m_end(std::move(end)) {
            ++(*this);
        }

        /// === FORWARD_ITERATOR ===
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
        /// ==========================
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
