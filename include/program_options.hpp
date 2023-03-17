#ifndef NODE_PROGRAM_OPTIONS
#define NODE_PROGRAM_OPTIONS

#include "types.hpp"
#include "argh.h"

namespace Cosmos {

    struct program_options {
        static const program_options read (const argh::parser &);

        string DatabasePath {};

    private:
        program_options () {}
    };

}

#endif
