
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>

#include "calc.hpp"

namespace Cosmos {

    struct calculator_actions {
        // Stack to store numbers and intermediate results
        static std::stack<int> values;

        // Function to parse numbers
        template <typename Input>
        static void number(const Input& in)
        {
            values.push(std::stoi(in.string()));
        }

        // Function to perform operations
        template <char Op>
        static void operation()
        {
            if (values.size() < 2)
            {
                throw std::runtime_error("Invalid expression");
            }

            int b = values.top(); values.pop();
            int a = values.top(); values.pop();

            switch (Op)
            {
                case '+': values.push(a + b); break;
                case '-': values.push(a - b); break;
                case '*': values.push(a * b); break;
                case '/': values.push(a / b); break;
                case '%': values.push(a % b); break;
                default: throw std::runtime_error("Unknown operation");
            }
        }
    };

    std::stack<int> calculator_actions::values;

    template <typename Rule>
    using calculator_action = tao::pegtl::nothing<Rule>;
/*
    template <>
    struct calculator_action<calc::number> : tao::pegtl::action_single<calculator_actions::number> {};

    template <char Op>
    struct calculator_action<tao::pegtl::one<Op>> : tao::pegtl::action_single<calculator_actions::operation<Op>> {};
*/
    int main(int argc, char** argv)
    {
        if (argc != 2)
        {
            std::cerr << "Usage: " << argv[0] << " <expression>" << std::endl;
            return 1;
        }

        try
        {
            tao::pegtl::memory_input<> input(argv[1], "expression");
            tao::pegtl::parse<calc::grammar, calculator_action>(input);

            if (calculator_actions::values.size() != 1)
            {
                throw std::runtime_error("Invalid expression");
            }

            std::cout << "Result: " << calculator_actions::values.top() << std::endl;
        }
        catch (const std::exception& ex)
        {
            std::cerr << "Error: " << ex.what() << std::endl;
            return 1;
        }

        return 0;
    }
}
