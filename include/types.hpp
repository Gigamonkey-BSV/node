#ifndef NODE_TYPES
#define NODE_TYPES

#include <string>

namespace Cosmos {
    template <typename X> using string_of = std::basic_string<X>;

    using string = string_of<char>;
}

#endif

