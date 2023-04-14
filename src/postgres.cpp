#include <iostream>
#include <regex>

#include "postgres.hpp"
#include "program_options.hpp"

namespace Cosmos {

    string postgres_URL::connect () const {
        if (!valid ()) throw exception {} << "invalid postgres URL \"" << *this << "\"";

        std::regex url_regex (R"(^postgres://([^:]+):([^@]+)@([^:]+):(\d+)/(\w+)$)");
        std::smatch url_match;

        if (!std::regex_search (*this, url_match, url_regex)) throw std::runtime_error ("Invalid postgres URL");

        std::string user = url_match[1];
        std::string password = url_match[2];
        std::string host = url_match[3];
        std::string port = url_match[4];
        std::string dbname = url_match[5];

        std::stringstream ss;
        ss << "host=" << url_match[3] << " port=" << url_match[4] << " dbname=" << url_match[5] << " password=" << url_match[2];

        return ss.str ();

    }
/*
    ptr<pqxx::connection> connect_to_database (const postgres_URL& url) {

        std::regex url_regex (R"(^postgres://([^:]+):([^@]+)@([^:]+):(\d+)/(\w+)$)");
        std::smatch url_match;

        if (!std::regex_search (url, url_match, url_regex)) throw std::runtime_error ("Invalid postgres URL");

        std::string user = url_match[1];
        std::string password = url_match[2];
        std::string host = url_match[3];
        std::string port = url_match[4];
        std::string dbname = url_match[5];

        std::stringstream ss;
        ss << "host=" << url_match[3] << " port=" << url_match[4] << " dbname=" << url_match[5] << " password=" << url_match[2];

        ptr<pqxx::connection> conn = std::make_shared<pqxx::connection> (ss.str());

        if (!conn->is_open ()) throw std::runtime_error ("Failed to connect to database");

        return conn;
    }*/
}
