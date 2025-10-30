#pragma once

#include <concepts>
#include <expected>
#include <iterator>
#include <optional>
#include <ranges>
#include <string_view>
#include <type_traits>
#include <variant>
#include <print>

#include "token.hpp"
#include "char_range.hpp"
#include "lexer_automaton.hpp"

namespace lexer {
struct InvalidPathError {
    std::string filename;
};

using LexError = std::variant<InvalidPathError>;
using Token = token::Token;

template <char_range R>
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

	token::Token m_tok{};
	lexer_automaton::LexerAutomaton m_automaton{};

        auto parse_token() -> Token {
	    while (m_it != m_end) {
		m_automaton.process(*m_it);
		m_it++;
		std::optional<std::expected<token::Token, lexer_automaton::LexError>> tok{m_automaton.get_token()};
                if (tok.has_value()) {
		    if (tok.value().has_value()) return tok.value().value();
		    std::println("{}",lexer_automaton::print_lex_error(tok.value().error()));
		}
            }
	    std::println("Error: EOF while lexing.");
	    return token::Eof{};
        }

       public:
        Iterator(r_iter_type begin, r_end_type end)
            : m_it(std::move(begin)), m_end(std::move(end)) {
            ++(*this);
        }

        /// === FORWARD_ITERATOR ===
        using difference_type = std::ptrdiff_t;
        using value_type = Token;
        using reference = const Token&;
        using iterator_category = std::input_iterator_tag;

        auto operator*() const -> const Token& { return m_tok; }

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

template <std::ranges::range R>
struct LexAdaptor : std::ranges::range_adaptor_closure<LexAdaptor<R>> {
    auto operator()(R src) const { return Lexer<R>{src}; }
};

template <std::ranges::range R>
constexpr LexAdaptor<R> lex{};

static_assert(std::ranges::range<lexer::Lexer<std::string_view>>);
static_assert(std::ranges::range<lexer::Lexer<std::ranges::istream_view<char>>>);

}  // namespace lexer
