// util.hpp
// misc templates that make life easier

#pragma once

namespace util {

// wrapper for visitor lambdas
template <class... Ts>
struct overloads : Ts... {
    using Ts::operator()...;
};

}  // namespace util
