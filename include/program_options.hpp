#ifndef NODE_PROGRAM_OPTIONS
#define NODE_PROGRAM_OPTIONS

#include "postgres.hpp"
#include "argh.h"

namespace Cosmos {

    struct program_options {
        static const program_options read (const argh::parser &);

        maybe<postgres_URL> DatabaseURL {};

        maybe<uint16> HTTPListenerPort {};

    private:
        program_options () {}
    };

    // everything that happens in the program after reading the program options.
    void run (const program_options &);

}

#endif
