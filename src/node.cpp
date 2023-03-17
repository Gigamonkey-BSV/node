#include <iostream>

#include "program_options.hpp"

namespace Cosmos {

    const char *Help = "help message here";

    const char *Version = "version 0.0.0";

    void run (const program_options &);

}

int main (int arg_count, char ** arg_values) {

    argh::parser command_line_parser;

    command_line_parser.parse (arg_count, arg_values);

    if (command_line_parser[{"--version"}]) std::cout << Cosmos::Version << "\n" << Cosmos::Help << std::endl;
    else if (command_line_parser[{"--help"}]) std::cout << Cosmos::Help << std::endl;
    else
        try {
            run (Cosmos::program_options::read (command_line_parser));
        } catch (std::exception &exception) {
            std::cout << "could not read program options: " << exception.what () << std::endl;
            return 1;
        } catch (...) {
            return 1;
        }

    return 0;
}

void Cosmos::run (const program_options &options) {
    while (true) {
        string command;
        std::getline (std::cin, command);
        std::cout << "read command " << command << std::endl;
    }
}
