#include "program_options.hpp"
#include "dotenv.h"

std::string get_option (const argh::parser &command_line, std::string option) {
    if (auto o = command_line (option); o) {
        std::string option_value;
        o >> option_value;

        return option_value;
    }

    char *val = std::getenv (option.c_str ());

    if (val == nullptr) //throw data::exception {} << "could not find option " << option;
        throw 0;
    return std::string {*val};
}

const Cosmos::program_options Cosmos::program_options::read (const argh::parser &command_line) {

    string env_path;

    if (auto option = command_line ("env_path", ".env"); option) option >> env_path;

    // it's not an error if this fails.
    dotenv::init (env_path.c_str ());

    program_options options {};

    options.DatabasePath = get_option (command_line, "db_url");

    if (options.DatabasePath == "") throw 0;

    return options;

}
