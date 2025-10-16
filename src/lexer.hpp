#pragma once

#include "token.hpp"
#include <algorithm>
#include <concepts>
#include <cstddef>
#include <expected>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <ranges>
#include <regex>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <print>

namespace lexer {
struct InvalidPathError {
    std::string filename;
};

using LexError = std::variant<InvalidPathError>;
using Token = token::Token;

auto rdfile(const std::filesystem::path& path) -> std::expected<std::string, InvalidPathError> {
    std::basic_ifstream<char> filestream{path};
    if (!filestream.is_open()) {
        return std::unexpected(InvalidPathError{path.string()});
    }

    std::ostringstream out;
    out << filestream.rdbuf();
    return out.str();
}

template <std::ranges::range R>
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

        unsigned int m_line_number{1};

        Token m_tok{};

        static auto is_whitespace(const char c) -> bool {
            return c == ' ' || c == '\t' || c == '\n';
        }

        // Return substring until c returns true on the current character
        // Does not consume that character
        auto consume(const std::function<bool(const char)>& c) -> std::string {
            std::string res{};
            while (!c(*m_it) && m_it != m_end) {
                res += *m_it;
                ++m_it;
            }
            return res;
        }

        auto consume(const std::function<bool(const char prev, const char c)>& c) -> std::string {
            std::string res{};
            char prev{};
            while (!c(prev, *m_it) && m_it != m_end) {
                res += *m_it;
                prev = *m_it;
                ++m_it;
            }
            return res;
        }

        auto parse_token() -> Token {
	    if (m_it == m_end) return token::Eof{};
            switch (*m_it) {
            case '\n':
                m_line_number++;
            case '\t':
            case ' ':
                m_it++;
                return parse_token();

            case '(':
                m_it++;
                return token::LParen{m_line_number};
            case ')':
                m_it++;
                return token::RParen{m_line_number};

            case ';':
                consume([](const char c) { return c == '\n'; });

            case '#':
                ++m_it;
                if (*m_it == '|') {
                    consume([](const char prev, const char c) { return prev == '|' && c == '#'; });
                }
		return parse_token();

            default:
                return token::Identifier{.m_line_number = m_line_number,
                                         .m_lexeme = consume(is_whitespace)};
            }
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

        auto operator==(std::default_sentinel_t other) const -> bool { return std::holds_alternative<token::Eof>(m_tok); }
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

template <std::ranges::range R> constexpr LexAdaptor<R> lex{};

static_assert(std::ranges::range<lexer::Lexer<std::string_view>>);
static_assert(std::ranges::range<lexer::Lexer<std::ranges::istream_view<char>>>);

} // namespace lexer
