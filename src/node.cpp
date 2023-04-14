#include <iostream>

#include "program_options.hpp"

namespace Cosmos {

    const char *Help =
        "This is a template for building c++ programs. "
        "\nThe program first checks for --help or --version options. "
        "If these are not given, it looks for an environment file by default named .env or whatever is given in "
        "option --env. In then searches for options, first in the command line and then in the env file, if one "
        "was found."
        "\nIt then searches for a postgres database url to connect to given by option \"db_url\". If one is "
        "found, it tries to connect to the database."
        "\nIt searches for option \"http_listener_port\". If an option is found, an HTTP server is started on "
        "the given port."
        "\nThe command line becomes a calculator app.";

    const char *Version = "version 0.0.0";

}

int main (int arg_count, char ** arg_values) {

    argh::parser command_line_parser;

    command_line_parser.parse (arg_count, arg_values);

    // display version.
    if (command_line_parser[{"--version"}]) std::cout << Cosmos::Version << std::endl;
    // display help.
    else if (command_line_parser[{"--help"}]) std::cout << Cosmos::Version << "\n" << Cosmos::Help << std::endl;
    // otherwise, run the program normally.
    else
        try {
            std::cout << "Welcome to node." << std::endl;
            run (Cosmos::program_options::read (command_line_parser));
        } catch (std::exception &exception) {
            std::cerr << exception.what () << std::endl;
            return 1;
        }

    return 0;
}

#include "calc.hpp"

namespace Cosmos {

    void run (const program_options &opts) {

        // try to connect to database
        // we don't do anything with the database so we just
        // forget about it after words.
        if (opts.DatabaseURL) {

            std::cout << "database url: " << *opts.DatabaseURL << std::endl;

            std::cout << "there is no database library yet, but if there was we would try to connect with the string "
                << opts.DatabaseURL->connect () << std::endl;
            /*
            ptr<pqxx::connection> connection = connect_to_database (*opts.DatabaseURL);

            if (connection->is_open ()) {
                std::cout << "Connected to the database!" << std::endl;
                connection->close ();
            } else {
                std::cout << "Cannot connect to the database." << std::endl;
            }*/
        }

        calc ();

    }
}
