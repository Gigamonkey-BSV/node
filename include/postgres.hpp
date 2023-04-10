#ifndef NODE_POSTGRES
#define NODE_POSTGRES

//#include <pqxx/pqxx>
#include "types.hpp"
#include "data/net/URL.hpp"

namespace Cosmos {

    struct postgres_URL : net::URL {
        using net::URL::URL;
    };

    //ptr<pqxx::connection> connect_to_database (const postgres_URL &);

}

#endif
