#include "char32_t_format.hpp"

#include <string_view>
#include <cstdint>
#include <format>

namespace char32_t_format {

std::expected<std::string, std::string> utf32_to_utf8(std::u32string_view u32str) {
    std::string out{};

    for (char32_t cp : u32str) {
        if (cp <= 0x7f) {
            out += static_cast<char>(cp);
        } else if (cp <= 0x7ff) {
            out += static_cast<char>(0xc0 | (cp >> 6));
            out += static_cast<char>(0x80 | (cp & 0x3f));
        } else if (cp <= 0xffff) {
            out += static_cast<char>(0xe0 | (cp >> 12));
            out += static_cast<char>(0x80 | ((cp >> 6) & 0x3f));
            out += static_cast<char>(0x80 | (cp & 0x3f));
        } else if (cp <= 0x10ffff) {
            out += static_cast<char>(0xf0 | (cp >> 18));
            out += static_cast<char>(0x80 | ((cp >> 12) & 0x3f));
            out += static_cast<char>(0x80 | ((cp >> 6) & 0x3f));
            out += static_cast<char>(0x80 | (cp & 0x3f));
        } else {
            return std::unexpected(std::format("Invalid unicode point: 0x{:08x}", std::bit_cast<uint32_t>(cp)));
        }
    }

    return out;
}
}
