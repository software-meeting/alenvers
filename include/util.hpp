// util.hpp
// misc templates that make life easier

#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <optional>
#include <ranges>
#include <utility>

namespace util {

// wrapper for visitor lambdas
template <class... Ts>
struct overloads : Ts... {
    using Ts::operator()...;
};

// range adaptor to normalise \r\n and \r to just \n
template <std::ranges::input_range R>
class newline_normaliser_adapter : std::ranges::view_interface<newline_normaliser_adapter<R>> {
   private:
    R m_input;

   public:
    class Iterator {
       private:
        using r_iter_type = std::ranges::iterator_t<R>;
        using r_end_type = std::ranges::sentinel_t<R>;

        r_iter_type m_it{};
        r_end_type m_end{};

        char m_current_char{};
        std::optional<char> m_cached_char{};

        bool m_at_end{false};

        void prime() {
            if (m_it == m_end && !m_cached_char) {
                m_at_end = true;
                return;
            }
            m_current_char = parse_char();
        }

        auto parse_char() -> char {
            if (m_cached_char) {
                char c = *m_cached_char;
                m_cached_char = std::nullopt;
                return c;
            }

            char curr = *m_it;
            m_it++;
            if (curr == '\r') {
                if (m_it != m_end) {
                    if (*m_it == '\n') {
                        m_it++;
                    } else {
                        m_cached_char = *m_it;
                        ++m_it;
                    }
                }
                return '\n';
            }

            return curr;
        }

       public:
        using difference_type = std::ptrdiff_t;
        using value_type = char;
        using iterator_concept = std::input_iterator_tag;

        Iterator() = default;

        Iterator(r_iter_type begin, r_end_type end)
            : m_it{std::move(begin)}, m_end{std::move(end)} {
            prime();
        }

        auto operator*() const -> const char& { return m_current_char; }

        auto operator++() -> Iterator& {
            if (m_it == m_end && !m_cached_char) {
                m_at_end = true;
            } else {
                m_current_char = parse_char();
            }
            return *this;
        }

        void operator++(int) { ++(*this); }

        auto operator==(std::default_sentinel_t other) const -> bool { return m_at_end; }
    };

   public:
    newline_normaliser_adapter() = default;

    template <typename T>
        requires std::constructible_from<R, T>
    explicit newline_normaliser_adapter(T&& input) : m_input{std::forward<T>(input)} {}

    auto begin() { return Iterator{std::begin(m_input), std::end(m_input)}; }

    auto end() { return std::default_sentinel; }
};

template <class R>
newline_normaliser_adapter(R&&) -> newline_normaliser_adapter<std::views::all_t<R>>;

}  // namespace util
