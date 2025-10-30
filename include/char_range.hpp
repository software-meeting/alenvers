#include <concepts>
#include <iterator>
#include <ranges>

template <typename T>
concept char_range =
    std::ranges::range<T> && std::same_as<std::iter_value_t<std::ranges::iterator_t<T>>, char> &&
    std::input_iterator<std::ranges::iterator_t<T>>;
