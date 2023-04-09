#include "program_options.hpp"
#include "dotenv.h"

namespace Cosmos {

    // first look in the command line options, then look in the env file.
    maybe<string> get_option (const argh::parser &command_line, string option) {
        if (auto o = command_line (string{"--"} + option); o) {
            string option_value;
            o >> option_value;

            return option_value;
        }

        char *val = std::getenv (option.c_str ());

        if (val == nullptr) return {};
        std::cout << "read option " << option << " from env: " << val << std::endl;
        return string {val};
    }

    // same as above, except return default value if no option is found in either case.
    string get_option (const argh::parser &command_line, string option, string default_value) {
        maybe<string> x = get_option (command_line, option);
        return x ? *x : default_value;
    }

    const program_options program_options::read (const argh::parser &command_line) {

        string env_path;

        if (auto option = command_line ("--env", ".env"); option) option >> env_path;

        // it's not an error if this fails.
        std::cout << "looking for env in " << env_path << std::endl;
        dotenv::init (env_path.c_str ());

        program_options options {};

        maybe<string> database_url = get_option (command_line, "db_url");

        if (database_url) {
            *options.DatabaseURL = postgres_URL {*database_url};
            //if (!options.DatabaseURL->valid ()) throw exception {} << "could not read database URL \"" << *database_url << "\"";
        }

        maybe<string> http_listener_port = get_option (command_line, "http_listener_port");

        if (http_listener_port) {
            std::stringstream ss {*http_listener_port};
            ss >> *options.HTTPListenerPort;
            if (*options.HTTPListenerPort == 0) throw exception {} << "invalid http listener port \"" << *http_listener_port << "\"";
        }

        return options;

    }
}
