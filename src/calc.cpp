
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>

#include "calc.hpp"
#include <data/numbers.hpp>

namespace Cosmos {

    namespace pegtl = tao::pegtl;

    struct evaluation {
        stack<Q> Stack;

        void read_symbol (const string &in) {
            throw exception {} << "read symbol " << in;
        }

        void read_string (const string &in) {
            throw exception {} << "read string literal " << in;
        }

        void read_number (const string &in) {
            Stack <<= Q {Z {in}};
        }

        void mul () {
            Stack = prepend (rest (rest (Stack)), first (Stack) * first (rest (Stack)));
        }

        void div () {
            auto denominator = math::nonzero<Q> {first (Stack)};
            if (!denominator.valid ()) throw exception {} << "division by zero";
            Stack = prepend (rest (rest (Stack)), first (rest (Stack)) / denominator);
        }

        void plus () {
            Stack = prepend (rest (rest (Stack)), first (Stack) + first (rest (Stack)));
        }

        void minus () {
            Stack = prepend (rest (rest (Stack)), first (Stack) - first (rest (Stack)));
        }
    };

    template <typename Rule> struct eval_action : pegtl::nothing<Rule> {};

    template <> struct eval_action<calc::number_lit> {
        template <typename Input>
        static void apply (const Input& in, evaluation &eval) {
            eval.read_number (in.string ());
        }
    };

    template <> struct eval_action<calc::string_lit> {
        template <typename Input>
        static void apply (const Input& in, evaluation &eval) {
            eval.read_string (in.string ());
        }
    };

    template <> struct eval_action<calc::symbol> {
        template <typename Input>
        static void apply (const Input& in, evaluation &eval) {
            eval.read_symbol (in.string ());
        }
    };

    template <> struct eval_action<calc::mul_op> {
        template <typename Input>
        static void apply (const Input& in, evaluation &eval) {
            eval.mul ();
        }
    };

    template <> struct eval_action<calc::div_op> {
        template <typename Input>
        static void apply (const Input& in, evaluation &eval) {
            eval.div ();
        }
    };

    template <> struct eval_action<calc::add_op> {
        template <typename Input>
        static void apply (const Input& in, evaluation &eval) {
            eval.plus ();
        }
    };

    template <> struct eval_action<calc::sub_op> {
        template <typename Input>
        static void apply (const Input& in, evaluation &eval) {
            eval.minus ();
        }
    };

    std::ostream &write (std::ostream &o, const Q &q) {
        o << q.Numerator;
        if (q.Denominator != 1) o << "/" << q.Denominator;
        return o;
    }

    void calc () {
        std::string input_str;
        std::cout << "calculator app engaged." << std::endl;
        while (true) {
            std::cout << "  input: ";

            if (!std::getline (std::cin, input_str)) break;
            if (input_str.empty ()) continue;

            try {
                tao::pegtl::memory_input<> input (input_str, "expression");
                evaluation eval {};
                tao::pegtl::parse<calc::grammar, eval_action> (input, eval);

                write (std::cout << " result: ", eval.Stack.first ()) << std::endl;

            } catch (const std::exception& ex) {
                std::cerr << "Error: " << ex.what () << std::endl;
            }
        }

    }
}
