#include <concepts>
#include <iterator>
#include <ranges>

namespace char_range {

template <typename T>
concept char_range =
    std::ranges::range<T> && std::same_as<std::iter_value_t<std::ranges::iterator_t<T>>, char> &&
    std::input_iterator<std::ranges::iterator_t<T>>;

using CharStreamRange =
    std::ranges::subrange<std::istreambuf_iterator<char>, std::istreambuf_iterator<char>>;

}  // namespace char_range
