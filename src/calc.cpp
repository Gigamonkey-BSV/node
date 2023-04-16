
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>

#include "calc.hpp"
#include <data/numbers.hpp>
#include <map>

namespace Diophant {

    using namespace data;

    struct expression;
    using value = ptr<expression>;

    struct expression {

        static value null ();
        static value boolean (bool b);
        static value rational (const data::Q &q);
        static value symbol (const data::string &x);
        static value string (const data::string &str);
        static value list (const data::list<value> ls);
        static value object (const data::list<data::entry<data::string, value>> x);

        static value negate (const value);
        static value plus (const value, const value);
        static value minus (const value, const value);
        static value times (const value, const value);
        static value divide (const value, const value);

        static value boolean_not (const value);
        static value boolean_and (const value, const value);
        static value boolean_or (const value, const value);

        static value intuitionistic_and (const value, const value);
        static value intuitionistic_or (const value, const value);
        static value intuitionistic_implies (const value, const value);

        virtual ~expression () {};

    };

    value apply (const value, const value);
    value operator - (const value);
    value operator + (const value, const value);
    value operator - (const value, const value);
    value operator * (const value, const value);
    value operator / (const value, const value);

    value operator == (const value, const value);
    value operator != (const value, const value);
    value operator <= (const value, const value);
    value operator >= (const value, const value);
    value operator < (const value, const value);
    value operator > (const value, const value);

    value operator ! (const value);
    value operator && (const value, const value);
    value operator || (const value, const value);

    // look up values of symbols in the map of values.
    value evaluate (const value, const std::map<data::string, value> &vars);

    struct evaluation {
        stack<value> Stack;
        std::map<data::string, value> &Vars;

        evaluation (std::map<data::string, value> &v) : Stack {}, Vars {v} {}

        void read_symbol (const data::string &in);
        void read_string (const data::string &in);
        void read_number (const data::string &in);

        void apply ();

        void negate ();
        void mul ();
        void div ();
        void plus ();
        void minus ();

        void equal ();
        void unequal ();
        void greater_equal ();
        void less_equal ();
        void greater ();
        void less ();

        void bool_and ();
        void bool_or ();

        void open_list ();
        void open_object ();
        void close_list ();
        void close_object ();
        void comma ();

        void set ();
    };
}

namespace Cosmos {

    namespace pegtl = tao::pegtl;

    std::ostream &write (std::ostream &o, const Diophant::value &v);

    template <typename Rule> struct eval_action : pegtl::nothing<Rule> {};

    template <> struct eval_action<calc::number_lit> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.read_number (in.string ());
        }
    };

    template <> struct eval_action<calc::string_body> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.read_string (in.string ());
        }
    };

    template <> struct eval_action<calc::symbol> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.read_symbol (in.string ());
        }
    };

    template <> struct eval_action<calc::unary_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.negate ();
        }
    };

    template <> struct eval_action<calc::mul_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.mul ();
        }
    };

    template <> struct eval_action<calc::div_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.div ();
        }
    };

    template <> struct eval_action<calc::add_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.plus ();
        }
    };

    template <> struct eval_action<calc::sub_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.minus ();
        }
    };

    template <> struct eval_action<calc::equal_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.equal ();
        }
    };

    template <> struct eval_action<calc::unequal_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.unequal ();
        }
    };

    template <> struct eval_action<calc::greater_equal_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.greater_equal ();
        }
    };

    template <> struct eval_action<calc::less_equal_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.less_equal ();
        }
    };

    template <> struct eval_action<calc::less_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.less ();
        }
    };

    template <> struct eval_action<calc::greater_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.greater ();
        }
    };

    template <> struct eval_action<calc::bool_and_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.bool_and ();
        }
    };

    template <> struct eval_action<calc::bool_or_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.bool_or ();
        }
    };

    template <> struct eval_action<calc::expression> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {}
    };

    template <> struct eval_action<calc::infer> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.set ();
        }
    };

    template <> struct eval_action<calc::statement> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            if (data::size (eval.Stack) == 1) {
                write (std::cout << "\n result: ", eval.Stack.first ()) << std::endl;
                eval.Stack = data::stack<Diophant::value> {};
            }
        }
    };
}

namespace Diophant {

    void inline evaluation::read_symbol (const data::string &in) {
        Stack <<= expression::symbol (in);
    }

    void inline evaluation::read_string (const data::string &in) {
        Stack <<= expression::string (in);
    }

    void inline evaluation::read_number (const data::string &in) {
        Stack <<= expression::rational (Q {Z {in}});
    }

    void inline evaluation::apply () {
        Stack = prepend (rest (rest (Stack)),
           Diophant::apply (evaluate (first (rest (Stack)), Vars), evaluate (first (Stack), Vars)));
    }

    void inline evaluation::negate () {
        Stack = prepend (rest (Stack), -evaluate (first (Stack), Vars));
    }

    void inline evaluation::mul () {
        Stack = prepend (rest (rest (Stack)), evaluate (first (rest (Stack)), Vars) * evaluate (first (Stack), Vars));
    }

    void inline evaluation::div () {
        Stack = prepend (rest (rest (Stack)), evaluate (first (rest (Stack)), Vars) / evaluate (first (Stack), Vars));
    }

    void inline evaluation::plus () {
        Stack = prepend (rest (rest (Stack)), evaluate (first (rest (Stack)), Vars) + evaluate (first (Stack), Vars));
    }

    void inline evaluation::minus () {
        Stack = prepend (rest (rest (Stack)), evaluate (first (rest (Stack)), Vars) - evaluate (first (Stack), Vars));
    }

    void inline evaluation::equal () {
        Stack = prepend (rest (rest (Stack)), evaluate (first (rest (Stack)), Vars) == evaluate (first (Stack), Vars));
    }

    void inline evaluation::unequal () {
        Stack = prepend (rest (rest (Stack)), evaluate (first (rest (Stack)), Vars) != evaluate (first (Stack), Vars));
    }

    void inline evaluation::greater_equal () {
        Stack = prepend (rest (rest (Stack)), evaluate (first (rest (Stack)), Vars) >= evaluate (first (Stack), Vars));
    }

    void inline evaluation::less_equal () {
        Stack = prepend (rest (rest (Stack)), evaluate (first (rest (Stack)), Vars) <= evaluate (first (Stack), Vars));
    }

    void inline evaluation::greater () {
        Stack = prepend (rest (rest (Stack)), evaluate (first (rest (Stack)), Vars) > evaluate (first (Stack), Vars));
    }

    void inline evaluation::less () {
        Stack = prepend (rest (rest (Stack)), evaluate (first (rest (Stack)), Vars) < evaluate (first (Stack), Vars));
    }

    void inline evaluation::bool_and () {
        Stack = prepend (rest (rest (Stack)), evaluate (first (rest (Stack)), Vars) && evaluate (first (Stack), Vars));
    }

    void inline evaluation::bool_or () {
        Stack = prepend (rest (rest (Stack)), evaluate (first (rest (Stack)), Vars) || evaluate (first (Stack), Vars));
    }

    struct boolean : expression {
        bool Value;
        boolean (const bool b) : Value {b} {}
    };

    struct symbol : expression {
        data::string Name;
        symbol (const data::string &x) : Name {x} {}
    };

    struct string : expression {
        data::string Value;
        string (const data::string &x) : Value {x} {}
    };

    struct rational : expression {
        data::Q Value;
        rational (const data::Q &q) : Value {q} {}
    };

    struct list : expression {
        data::list<value> Value;
        list (data::list<value> v) : Value {v} {}
    };

    struct negate : expression {
        value Value;
    };

    struct plus : expression {
        value Left;
        value Right;
    };

    struct minus : expression {
        value Left;
        value Right;
    };

    struct times : expression {
        value Left;
        value Right;
    };

    struct divide : expression {
        value Left;
        value Right;
    };

    struct intuitionistic_and : expression {
        value Left;
        value Right;
    };

    struct intuitionistic_or : expression {
        value Left;
        value Right;
    };

    struct intuitionistic_implies : expression {
        value Left;
        value Right;
    };

    struct object : expression {
        data::list<entry<data::string, value>> Value;
        object (data::list<entry<data::string, value>> v) : Value {v} {}
    };

    value inline expression::null () {
        return value {nullptr};
    }

    value inline expression::boolean (bool b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::boolean> (b));
    }

    value inline expression::rational (const Q &q) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::rational> (q));
    }

    value inline expression::symbol (const data::string &x) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::symbol> (x));
    }

    value inline expression::string (const data::string &str) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::string> (str));
    }

    value inline expression::list (const data::list<value> ls) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::list> (ls));
    }

    value inline expression::object (const data::list<entry<data::string, value>> x) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::object> (x));
    }

    value inline operator - (const value v) {
        if (auto p = std::dynamic_pointer_cast<const rational> (v); p != nullptr)
            return expression::rational (-p->Value);

        throw exception {} << "invalid operation";
    }

    value inline operator + (const value v, const value w) {
        auto a = std::dynamic_pointer_cast<const rational> (v);
        auto b = std::dynamic_pointer_cast<const rational> (w);
        if (a != nullptr && b != nullptr) return expression::rational (a->Value + b->Value);

        throw exception {} << "invalid operation";
    }

    value inline operator - (const value v, const value w) {
        auto a = std::dynamic_pointer_cast<const rational> (v);
        auto b = std::dynamic_pointer_cast<const rational> (w);
        if (a != nullptr && b != nullptr) return expression::rational (a->Value - b->Value);

        throw exception {} << "invalid operation";
    }

    value inline operator * (const value v, const value w) {
        auto a = std::dynamic_pointer_cast<const rational> (v);
        auto b = std::dynamic_pointer_cast<const rational> (w);
        if (a != nullptr && b != nullptr) return expression::rational (a->Value * b->Value);

        throw exception {} << "invalid operation";
    }

    value operator / (const value v, const value w) {
        auto a = std::dynamic_pointer_cast<const rational> (v);
        auto b = std::dynamic_pointer_cast<const rational> (w);
        if (a != nullptr && b != nullptr) return expression::rational (a->Value / math::nonzero<Q> {b->Value});

        throw exception {} << "invalid operation";
    }

    value operator == (const value v, const value w) {
        if (v.get () == nullptr && w.get () == nullptr) return expression::boolean (true);

        auto ba = std::dynamic_pointer_cast<const boolean> (v);
        auto bb = std::dynamic_pointer_cast<const boolean> (w);

        if (ba != nullptr && bb != nullptr) return expression::boolean (ba->Value == bb->Value);

        auto ra = std::dynamic_pointer_cast<const rational> (v);
        auto rb = std::dynamic_pointer_cast<const rational> (w);

        if (ra != nullptr && rb != nullptr) return expression::boolean (ra->Value == rb->Value);

        throw exception {} << "invalid operation";

    }

    value inline operator != (const value v, const value w) {
        if (v.get () == nullptr && w.get () == nullptr) return expression::boolean (false);

        auto ba = std::dynamic_pointer_cast<const boolean> (v);
        auto bb = std::dynamic_pointer_cast<const boolean> (w);

        if (ba != nullptr && bb != nullptr) return expression::boolean (ba->Value != bb->Value);

        auto ra = std::dynamic_pointer_cast<const rational> (v);
        auto rb = std::dynamic_pointer_cast<const rational> (w);

        if (ra != nullptr && rb != nullptr) return expression::boolean (ra->Value != rb->Value);

        throw exception {} << "invalid operation";
    }

    value inline operator <= (const value v, const value w) {
        auto ra = std::dynamic_pointer_cast<const rational> (v);
        auto rb = std::dynamic_pointer_cast<const rational> (w);

        if (ra != nullptr && rb != nullptr) return expression::boolean (ra->Value <= rb->Value);

        throw exception {} << "invalid operation";
    }

    value inline operator >= (const value v, const value w) {
        auto ra = std::dynamic_pointer_cast<const rational> (v);
        auto rb = std::dynamic_pointer_cast<const rational> (w);

        if (ra != nullptr && rb != nullptr) return expression::boolean (ra->Value <= rb->Value);

        throw exception {} << "invalid operation";
    }

    value inline operator < (const value v, const value w) {
        auto ra = std::dynamic_pointer_cast<const rational> (v);
        auto rb = std::dynamic_pointer_cast<const rational> (w);

        if (ra != nullptr && rb != nullptr) return expression::boolean (ra->Value < rb->Value);

        throw exception {} << "invalid operation";
    }

    value inline operator > (const value v, const value w) {
        auto ra = std::dynamic_pointer_cast<const rational> (v);
        auto rb = std::dynamic_pointer_cast<const rational> (w);

        if (ra != nullptr && rb != nullptr) return expression::boolean (ra->Value < rb->Value);

        throw exception {} << "invalid operation";
    }

    value inline operator && (const value v, const value w) {
        auto a = std::dynamic_pointer_cast<const boolean> (v);
        auto b = std::dynamic_pointer_cast<const boolean> (w);
        if (a != nullptr && b != nullptr) return expression::boolean (a->Value && b->Value);

        throw exception {} << "invalid operation";
    }

    value inline operator || (const value v, const value w) {
        auto a = std::dynamic_pointer_cast<const boolean> (v);
        auto b = std::dynamic_pointer_cast<const boolean> (w);
        if (a != nullptr && b != nullptr) return expression::boolean (a->Value || b->Value);

        throw exception {} << "invalid operation";
    }

    void evaluation::set () {
        auto v = std::dynamic_pointer_cast<const symbol> (first (rest (Stack)));
        if (v == nullptr) throw exception {} << "invalid operation";
        const data::string &var = v->Name;
        if (Vars.find (var) != Vars.end ()) throw exception {} << "variable " << var << " is already defined!";
        auto val = first (Stack);
        Vars.insert (std::pair {var, val});
        Stack = prepend (rest (rest (Stack)), val);
    }

    value evaluate (const value v, const std::map<data::string, value> &vars) {
        auto a = std::dynamic_pointer_cast<const symbol> (v);

        if (a != nullptr) {
            auto x = vars.find (a->Name);
            if (x == vars.end ()) throw exception {} << "undefined symbol " << a->Name;

            return evaluate (x->second, vars);
        }

        return v;
    }

}

namespace Cosmos {

    void calc () {
        std::string input_str;
        std::cout << "\nCalculator app engaged! The calculator app supports rational arithmetic. You can also set variables." << std::endl;

        std::map<string, Diophant::value> vars;

        // defined symbols
        vars.insert (std::pair {"null", Diophant::expression::null ()});
        vars.insert (std::pair {"true", Diophant::expression::boolean (true)});
        vars.insert (std::pair {"false", Diophant::expression::boolean (false)});

        while (true) {
            std::cout << "\n input: ";

            if (!std::getline (std::cin, input_str)) break;
            if (input_str.empty ()) continue;

            try {
                tao::pegtl::memory_input<> input (input_str, "expression");
                Diophant::evaluation eval {vars};
                tao::pegtl::parse<calc::grammar, eval_action> (input, eval);
            } catch (const std::exception& ex) {
                std::cerr << "Error: " << ex.what () << std::endl;
            }
        }

    }

    std::ostream &write (std::ostream &o, const Q &q) {
        o << q.Numerator;
        if (q.Denominator != 1) o << "/" << q.Denominator;
        return o;
    }

    std::ostream &write (std::ostream &o, const list<Diophant::value> &ls) {
        o << "[";
        if (!data::empty (ls)) {
            write (o, first (ls));
            for (const auto v : ls) write (o << ", ", v);
        }
        return o << "]";
    }

    std::ostream &write (std::ostream &o, const list<entry<string, Diophant::value>> &x) {
        o << "{";
        if (!data::empty (x)) {
            auto e = first (x);
            write (o << e.Key << ": ", e.Value);
            for (const auto e : x) write (o << ", " << e.Key << ": ", e.Value);
        }
        return o << "}";
    }

    std::ostream &write (std::ostream &o, const Diophant::value &v) {

        if (v.get () == nullptr) return o << "null";

        if (auto p = std::dynamic_pointer_cast<Diophant::boolean> (v)) return o << std::boolalpha << p->Value;

        if (auto p = std::dynamic_pointer_cast<Diophant::symbol> (v)) return o << p->Name;

        if (auto p = std::dynamic_pointer_cast<Diophant::string> (v)) return o << "\"" << p->Value << "\"";

        if (auto p = std::dynamic_pointer_cast<Diophant::rational> (v)) return write (o, p->Value);

        if (auto p = std::dynamic_pointer_cast<Diophant::list> (v)) return write (o, p->Value);

        if (auto p = std::dynamic_pointer_cast<Diophant::object> (v)) return write (o, p->Value);

        return o;
    }
}
