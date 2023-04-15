
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>

#include "calc.hpp"
#include <data/numbers.hpp>
#include <map>

namespace Cosmos {

    namespace pegtl = tao::pegtl;

    struct value : either<Q, data::string> {

        static value number (const Q &q) {
            return value {q};
        }

        static value symbol (const string &x) {
            return value {x};
        }

        static value string (const string &) {
            throw exception {} << "we don't do strings yet ";
        }

        Q get (const std::map<data::string, value> &Vars) const {
            if (std::holds_alternative<data::string> (*this)) {

                data::string var = std::get<data::string> (*this);
                auto val = Vars.find (var);

                if (val == Vars.end ()) throw exception {} << "no variable " << var;

                return val->second.get (Vars);
            }

            return std::get<Q> (*this);
        }

    private:
        either<Q, data::string>::either;
    };

    struct evaluation {
        stack<value> Stack;
        std::map<string, value> &Vars;

        evaluation (std::map<string, value> &v) : Stack {}, Vars {v} {}

        void read_symbol (const string &in) {
            Stack <<= value::symbol (in);
        }

        void read_string (const string &in) {
            throw exception {} << "read string literal " << in;
        }

        void read_number (const string &in) {
            Stack <<= value::number (Q {Z {in}});
        }

        void negate () {
            Stack = prepend (rest (Stack), value::number (-first (Stack).get (Vars)));
        }

        void mul () {
            Stack = prepend (rest (rest (Stack)), value::number (first (rest (Stack)).get (Vars) * first (Stack).get (Vars)));
        }

        void div () {
            auto denominator = math::nonzero<Q> {first (Stack).get (Vars)};
            if (!denominator.valid ()) throw exception {} << "division by zero";
            Stack = prepend (rest (rest (Stack)), value::number (first (rest (Stack)).get (Vars) / denominator));
        }

        void plus () {
            Stack = prepend (rest (rest (Stack)), value::number (first (rest (Stack)).get (Vars) + first (Stack).get (Vars)));
        }

        void minus () {
            Stack = prepend (rest (rest (Stack)), value::number (first (rest (Stack)).get (Vars) - first (Stack).get (Vars)));
        }

        void set () {
            data::string var = std::get<data::string> (first (rest (Stack)));
            if (Vars.find (var) != Vars.end ()) throw exception {} << "variable " << var << " is already defined!";
            auto val = first (Stack);
            Vars.insert(std::pair {var, val});
            Stack = stack<value> {val};
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

    template <> struct eval_action<calc::unary_op> {
        template <typename Input>
        static void apply (const Input& in, evaluation &eval) {
            eval.negate ();
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

    template <> struct eval_action<calc::expression> {
        template <typename Input>
        static void apply (const Input& in, evaluation &eval) {}
    };

    template <> struct eval_action<calc::set> {
        template <typename Input>
        static void apply (const Input& in, evaluation &eval) {
            eval.set ();
        }
    };

    std::ostream &write (std::ostream &o, const value &v) {
        Q q = std::get<Q> (v);
        o << q.Numerator;
        if (q.Denominator != 1) o << "/" << q.Denominator;
        return o;
    }

    template <> struct eval_action<calc::statement> {
        template <typename Input>
        static void apply (const Input& in, evaluation &eval) {
            if (!data::empty (eval.Stack)) write (std::cout << "\n result: ", eval.Stack.first ()) << std::endl;
        }
    };

    void calc () {
        std::string input_str;
        std::cout << "\nCalculator app engaged! The calculator app supports rational arithmetic. You can also set variables." << std::endl;

        std::map<string, value> vars;

        while (true) {
            std::cout << "\n input: ";

            if (!std::getline (std::cin, input_str)) break;
            if (input_str.empty ()) continue;

            try {
                tao::pegtl::memory_input<> input (input_str, "expression");
                evaluation eval {vars};
                tao::pegtl::parse<calc::grammar, eval_action> (input, eval);

            } catch (const std::exception& ex) {
                std::cerr << "Error: " << ex.what () << std::endl;
            }
        }

    }
}
