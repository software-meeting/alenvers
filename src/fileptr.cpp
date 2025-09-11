#include "fileptr.hpp"

#include <cstdio>

namespace fileptr {
void _file_closer::operator()(std::FILE* f) const {
    if (f)
        std::fclose(f);
}
} // namespace fileptr
