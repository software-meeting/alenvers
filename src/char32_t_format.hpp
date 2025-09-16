#ifndef CHAR32_T_FORMAT
#define CHAR32_T_FORMAT

#include <expected>
#include <string>
#include <string_view>

namespace char32_t_format {
std::expected<std::string, std::string> utf32_to_utf8(std::u32string_view u32str);
}

#endif
