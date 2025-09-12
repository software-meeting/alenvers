#include <codecvt>
#include <locale>
namespace sad {
    class conv {
    public:
	// DO NOT REMOVE INLINE
	static inline std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf32_to_utf8;
    };
}
