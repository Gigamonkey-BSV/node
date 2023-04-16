
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

        static value apply (const value, const value);

        static value negate (const value);
        static value plus (const value, const value);
        static value minus (const value, const value);
        static value times (const value, const value);
        static value power (const value, const value);
        static value divide (const value, const value);

        static value equal (const value, const value);
        static value unequal (const value, const value);
        static value greater_equal (const value, const value);
        static value less_equal (const value, const value);
        static value greater (const value, const value);
        static value less (const value, const value);

        static value boolean_not (const value);
        static value boolean_and (const value, const value);
        static value boolean_or (const value, const value);

        static value arrow (const value, const value);

        static value intuitionistic_and (const value, const value);
        static value intuitionistic_or (const value, const value);
        static value intuitionistic_implies (const value, const value);

        virtual ~expression () {};

        virtual std::ostream &write (std::ostream &) const = 0;

        virtual uint32 precedence () const {
            return 0;
        }

        virtual value evaluate (const std::map<data::string, value> &vars) const {
            return *this;
        };

        value virtual operator () const;

    };

    value operator - (const value);
    value operator + (const value, const value);
    value operator - (const value, const value);
    value operator * (const value, const value);
    value operator ^ (const value, const value);
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

    value evaluate (const value v, const std::map<data::string, value> &vars);

    struct evaluation {
        stack<value> Stack;
        std::map<data::string, value> &Vars;

        evaluation (std::map<data::string, value> &v) : Stack {}, Vars {v} {}

        void read_symbol (const data::string &in);
        void read_string (const data::string &in);
        void read_number (const data::string &in);

        void open_list ();
        void open_object ();
        void close_list ();
        void close_object ();
        void comma ();

        void apply ();

        void negate ();
        void mul ();
        void pow ();
        void div ();
        void plus ();
        void minus ();

        void equal ();
        void unequal ();
        void greater_equal ();
        void less_equal ();
        void greater ();
        void less ();

        void boolean_not ();
        void boolean_and ();
        void boolean_or ();

        void arrow ();

        void intuitionistic_and ();
        void intuitionistic_or ();
        void intuitionistic_implies ();

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

    template <> struct eval_action<calc::negate_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.negate ();
        }
    };

    template <> struct eval_action<calc::bool_not_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.boolean_not ();
        }
    };

    template <> struct eval_action<calc::mul_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.mul ();
        }
    };

    template <> struct eval_action<calc::pow_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.pow ();
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
            eval.boolean_and ();
        }
    };

    template <> struct eval_action<calc::bool_or_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.boolean_or ();
        }
    };

    template <> struct eval_action<calc::arrow_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.arrow ();
        }
    };

    template <> struct eval_action<calc::intuitionistic_and_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.intuitionistic_and ();
        }
    };

    template <> struct eval_action<calc::intuitionistic_or_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.intuitionistic_or ();
        }
    };

    template <> struct eval_action<calc::intuitionistic_implies_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.intuitionistic_implies ();
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
                write (std::cout << "\n result: ", evaluate (eval.Stack.first (), eval.Vars)) << std::endl;
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
        Stack = prepend (rest (rest (Stack)), expression::apply (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::negate () {
        Stack = prepend (rest (Stack), expression::negate (first (Stack)));
    }

    void inline evaluation::boolean_not () {
        Stack = prepend (rest (Stack), expression::boolean_not (first (Stack)));
    }

    void inline evaluation::mul () {
        Stack = prepend (rest (rest (Stack)), expression::times (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::div () {
        Stack = prepend (rest (rest (Stack)), expression::divide (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::plus () {
        Stack = prepend (rest (rest (Stack)), expression::plus (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::minus () {
        Stack = prepend (rest (rest (Stack)), expression::minus (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::equal () {
        Stack = prepend (rest (rest (Stack)), expression::equal (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::unequal () {
        Stack = prepend (rest (rest (Stack)), expression::unequal (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::greater_equal () {
        Stack = prepend (rest (rest (Stack)), expression::greater_equal (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::less_equal () {
        Stack = prepend (rest (rest (Stack)), expression::less_equal (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::greater () {
        Stack = prepend (rest (rest (Stack)), expression::greater (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::less () {
        Stack = prepend (rest (rest (Stack)), expression::less (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::boolean_and () {
        Stack = prepend (rest (rest (Stack)), expression::boolean_and (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::boolean_or () {
        Stack = prepend (rest (rest (Stack)), expression::boolean_or (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::arrow () {
        Stack = prepend (rest (rest (Stack)), expression::arrow (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::intuitionistic_and () {
        Stack = prepend (rest (rest (Stack)), expression::intuitionistic_and (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::intuitionistic_or () {
        Stack = prepend (rest (rest (Stack)), expression::intuitionistic_or (first (rest (Stack)), first (Stack)));
    }

    void inline evaluation::intuitionistic_implies () {
        Stack = prepend (rest (rest (Stack)), expression::intuitionistic_implies (first (rest (Stack)), first (Stack)));
    }

    value inline evaluate (const value v, const std::map<data::string, value> &vars) {
        if (value == nullptr) return v;

        return v->evaluate (vars);
    }

    struct boolean : expression {
        bool Value;
        boolean (const bool b) : Value {b} {}

        std::ostream &write (std::ostream &o) override {
            return << std::boolalpha << Value;
        }
    };

    struct symbol : expression {
        data::string Name;
        symbol (const data::string &x) : Name {x} {}

        std::ostream &write (std::ostream &o) override {
            return << Name;
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto x = vars.find (a->Name);
            if (x == vars.end ()) throw exception {} << "undefined symbol " << a->Name;

            return evaluate (x->second, vars);
        };
    };

    struct string : expression {
        data::string Value;
        string (const data::string &x) : Value {x} {}

        std::ostream &write (std::ostream &o) override {
            return << "\"" << Value << "\"";
        }
    };

    struct rational : expression {
        data::Q Value;
        rational (const data::Q &q) : Value {q} {}

        std::ostream &write (std::ostream &o) override {
            o << Value.Numerator;
            if (Value.Denominator != 1) o << "/" << Value.Denominator;
            return o;
        }
    };

    struct list : expression {
        data::list<value> Value;
        list (data::list<value> v) : Value {v} {}

        std::ostream &write (std::ostream &o) override {
            o << "[";
            if (!data::empty (Value)) {
                write (o, first (Value));
                for (const auto v : Value) write (o << ", ", v);
            }
            return o << "]";
        }
    };

    struct object : expression {
        data::list<entry<data::string, value>> Value;
        object (data::list<entry<data::string, value>> v) : Value {v} {}

        std::ostream &write (std::ostream &o) override {
            o << "{";
            if (!data::empty (x)) {
                auto e = first (x);
                write (o << e.Key << ": ", e.Value);
                for (const auto e : x) write (o << ", " << e.Key << ": ", e.Value);
            }
            return o << "}";
        }
    };

    struct apply : expression {
        value Left;
        value Right;
        apply (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 100;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct negate : expression {
        value Value;
        negate (const value &v) : Value {v} {}

        uint32 precedence () const override {
            return 200;
        }

        value evaluate (const std::map<data::string, value> &vars) const {
            return -value->evaluate (vars);
        };

        std::ostream &write (std::ostream &o) override {
            o << "-";
            if (Value->precedence () > precedence ()) o << "(" << Value->write () << ")";
            else o << Value->write ();
        }
    };

    struct boolean_not : expression {
        value Value;
        boolean_not (const value &v) : Value {v} {}

        uint32 precedence () const override {
            return 200;
        }

        std::ostream &write (std::ostream &o) override {
            o << "!";
            if (Value->precedence () > precedence ()) o << "(" << Value->write () << ")";
            else o << Value->write ();
        }
    };

    struct plus : expression {
        value Left;
        value Right;
        plus (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 300;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " + ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct minus : expression {
        value Left;
        value Right;
        minus (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 400;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " - ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct times : expression {
        value Left;
        value Right;
        times (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 500;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " * ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct divide : expression {
        value Left;
        value Right;
        divide (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 600;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " / ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct equal : expression {
        value Left;
        value Right;
        equal (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 700;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " == ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct unequal : expression {
        value Left;
        value Right;
        unequal (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 700;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " != ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct greater_equal : expression {
        value Left;
        value Right;
        greater_equal (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 700;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " >= ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct less_equal : expression {
        value Left;
        value Right;
        less_equal (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 700;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " <= ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct greater : expression {
        value Left;
        value Right;
        greater (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 700;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " > ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct less : expression {
        value Left;
        value Right;
        less (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 700;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " < ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct boolean_and : expression {
        value Left;
        value Right;
        boolean_and (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 800;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " && ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct boolean_or : expression {
        value Left;
        value Right;
        boolean_or (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 900;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " || ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct arrow : expression {
        value Left;
        value Right;
        arrow (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 1000;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " -> ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct intuitionistic_and : expression {
        value Left;
        value Right;
        intuitionistic_and (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 1100;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " & ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct intuitionistic_or : expression {
        value Left;
        value Right;
        intuitionistic_or (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 1200;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " | ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
    };

    struct intuitionistic_implies : expression {
        value Left;
        value Right;
        intuitionistic_implies (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 1300;
        }

        std::ostream &write (std::ostream &o) override {
            if (Left->precedence () > precedence ()) o << "(" << Left->write () << ")";
            else o << Left->write ();
            o << " => ";
            if (Right->precedence () > precedence ()) o << "(" << Right->write () << ")";
            else o << Right->write ();
        }
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

    value inline expression::apply (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::apply> (a, b));
    }

    value inline expression::operator (const value x) const {
        return apply (this, x);
    }

    value inline expression::negate (const value x) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::negate> (x));
    }

    value inline expression::plus (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::plus> (a, b));
    }

    value inline expression::minus (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::minus> (a, b));
    }

    value inline expression::times (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::times> (a, b));
    }

    value inline expression::divide (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::divide> (a, b));
    }

    value inline expression::equal (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::equal> (a, b));
    }

    value inline expression::unequal (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::unequal> (a, b));
    }

    value inline expression::greater_equal (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::greater_equal> (a, b));
    }

    value inline expression::less_equal (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::less_equal> (a, b));
    }

    value inline expression::greater (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::greater> (a, b));
    }

    value inline expression::less (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::less> (a, b));
    }

    value inline expression::boolean_not (const value x) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::boolean_not> (x));
    }

    value inline expression::boolean_and (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::boolean_and> (a, b));
    }

    value inline expression::boolean_or (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::boolean_or> (a, b));
    }

    value inline expression::arrow (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::arrow> (a, b));
    }

    value inline expression::intuitionistic_and (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::intuitionistic_and> (a, b));
    }

    value inline expression::intuitionistic_or (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::intuitionistic_or> (a, b));
    }

    value inline expression::intuitionistic_implies (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::intuitionistic_implies> (a, b));
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


    std::ostream &write (std::ostream &o, const Diophant::value &v) {

        if (v.get () == nullptr) return o << "null";

        if (auto p = std::dynamic_pointer_cast<Diophant::boolean> (v)) return o << std::boolalpha << p->Value;

        if (auto p = std::dynamic_pointer_cast<Diophant::symbol> (v)) return o << p->Name;

        if (auto p = std::dynamic_pointer_cast<Diophant::string> (v)) return o << "\"" << p->Value << "\"";

        if (auto p = std::dynamic_pointer_cast<Diophant::rational> (v)) return write (o, p->Value);

        if (auto p = std::dynamic_pointer_cast<Diophant::list> (v)) return write (o, p->Value);

        if (auto p = std::dynamic_pointer_cast<Diophant::object> (v)) return write (o, p->Value);

        throw exception {} << "cannot display";

        return o;
    }
}
