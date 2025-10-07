#ifndef LEXER_HPP
#define LEXER_HPP

#include "token.hpp"
#include <algorithm>
#include <concepts>
#include <cstddef>
#include <expected>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <ranges>
#include <string_view>
#include <variant>

namespace lexer {
struct InvalidPathError {
    std::string filename;
};

using LexError = std::variant<InvalidPathError>;
using Token = token::Token;
using TokenType = token::TokenType;

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
             std::forward_iterator<std::ranges::iterator_t<R>>
class Lexer {
  private:
    using r_iter_type = std::ranges::iterator_t<const R>;
    const R m_src;

    class Iterator {
      private:
        r_iter_type m_it{};
        r_iter_type m_end{};
        Token m_tok{};

        /// TODO: Scanner logic
        auto parse_token() -> Token {
            m_it++;
            if (m_it == m_end)
                return Token{TokenType::END, '0', 42};
            return Token{TokenType::IDENTIFIER, "Ur mum", 69};
        }

      public:
        Iterator(r_iter_type begin, r_iter_type end) : m_it(begin), m_end(end) {
            m_tok = parse_token();
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
            m_tok = parse_token();
            return old;
        }

        auto operator==(const Iterator& other) const -> bool { return m_it == other.m_it; }
        /// ==========================
    };

    static_assert(std::input_iterator<Iterator>);

  public:
    explicit Lexer(R src) : m_src(std::move(src)) {}

    [[nodiscard]] auto begin() const {
        return Iterator(std::ranges::begin(m_src), std::ranges::end(m_src));
    }

    [[nodiscard]] auto end() const {
        return Iterator(std::ranges::end(m_src), std::ranges::end(m_src));
    }

};

    // TODO: make it model std::range correctly
    // this shit is cooked
    static_assert(std::ranges::range<Lexer<std::string_view>>);

} // namespace lexer

#endif
