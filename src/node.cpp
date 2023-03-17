#include "argh.h"
#include <iostream>

#include "program_options.hpp"

const char *Help = "help message here";

const char *Version = "version 0.0.0";

void run (const program_options &);

int main (int arg_count, char ** arg_values) {

    argh::parser command_line_parser;

    command_line_parser.parse (argc, argv);

    if (command_line_parser[{"--version"}]) std::cout << Version << "\n" << Help << std::endl;
    else (command_line_parser[{"--help"}]) std::cout << Help << std::endl;
    else
        try {
            run (program_options::read (command_line_parser));
        } catch (std::exception &exception) {
            std::cout << "could not read program options: " << exception.what () << std::endl;
            return 1;
        }

    return 0;
}

void run (const program_options &options) {
    // connect to database

    // connect to internet
}
