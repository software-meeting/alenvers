#ifndef FILEPTR_HPP
#define FILEPTR_HPP

#include <cstdio>
#include <memory>

namespace fileptr {
    struct _file_closer {
	void operator()(std::FILE* f) const;
    };

    using FilePtr = std::unique_ptr<std::FILE, _file_closer>;
}

#endif
