#ifndef NODE_DATABASE
#define NODE_DATABASE

#include <pqxx/pqxx>
#include "types.hpp"

namespace Cosmos {

    struct postgres_URL : net::URL {
        using net::URL::URL;

        bool valid () const {
            return net::URL::valid () && this->protocol () == net::protocol {"postgres"} && this->user_name_pass ();
        }

        data::string connect_command () const;
    };

    ptr<pqxx::connection> connect_to_database (const postgres_URL &);

}

#endif
