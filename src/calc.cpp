
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>

#include "calc.hpp"
#include <data/numbers.hpp>
#include <data/for_each.hpp>
#include <map>

namespace Diophant {

    using namespace data;

    struct expression;
    using value = const ptr<const expression>;

    struct expression : std::enable_shared_from_this<expression> {

        static value null ();
        static value boolean (bool b);
        static value rational (const data::Q &q);
        static value symbol (const data::string &x);
        static value string (const data::string &str);
        static value list (const data::list<value> &ls);
        static value object (const data::list<data::entry<data::string, value>> &x);

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

        data::string write () const {
            std::stringstream ss;
            write (ss);
            return ss.str ();
        }

        virtual uint32 precedence () const {
            return 0;
        }

        virtual value evaluate (const std::map<data::string, value> &vars) const {
            return this->shared_from_this ();
        };

        virtual value operator () (const value) const;
        virtual value operator - () const;
        virtual value operator ! () const;

        virtual value operator + (const value) const;
        virtual value operator - (const value) const;
        virtual value operator * (const value) const;
        virtual value operator / (const value) const;
        virtual value operator ^ (const value) const;

        virtual value operator == (const value) const;
        virtual value operator != (const value) const;
        virtual value operator <= (const value) const;
        virtual value operator >= (const value) const;
        virtual value operator < (const value) const;
        virtual value operator > (const value) const;

        virtual value operator && (const value) const;
        virtual value operator || (const value) const;
        virtual value arrow (const value) const;

        virtual value operator & (const value) const;
        virtual value operator | (const value) const;
        virtual value implies (const value) const;

    };

    value evaluate (value v, const std::map<data::string, value> &vars);

    std::ostream inline &operator << (std::ostream &o, value v) {
        return v->write (o);
    }

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

namespace Diophant {

    namespace pegtl = tao::pegtl;

    template <typename Rule> struct eval_action : pegtl::nothing<Rule> {};

    template <> struct eval_action<parse::number_lit> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.read_number (in.string ());
        }
    };

    template <> struct eval_action<parse::string_body> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.read_string (in.string ());
        }
    };

    template <> struct eval_action<parse::symbol> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.read_symbol (in.string ());
        }
    };

    template <> struct eval_action<parse::negate_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.negate ();
        }
    };

    template <> struct eval_action<parse::bool_not_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.boolean_not ();
        }
    };

    template <> struct eval_action<parse::mul_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.mul ();
        }
    };

    template <> struct eval_action<parse::pow_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.pow ();
        }
    };

    template <> struct eval_action<parse::div_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.div ();
        }
    };

    template <> struct eval_action<parse::add_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.plus ();
        }
    };

    template <> struct eval_action<parse::sub_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.minus ();
        }
    };

    template <> struct eval_action<parse::equal_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.equal ();
        }
    };

    template <> struct eval_action<parse::unequal_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.unequal ();
        }
    };

    template <> struct eval_action<parse::greater_equal_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.greater_equal ();
        }
    };

    template <> struct eval_action<parse::less_equal_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.less_equal ();
        }
    };

    template <> struct eval_action<parse::less_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.less ();
        }
    };

    template <> struct eval_action<parse::greater_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.greater ();
        }
    };

    template <> struct eval_action<parse::bool_and_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.boolean_and ();
        }
    };

    template <> struct eval_action<parse::bool_or_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.boolean_or ();
        }
    };

    template <> struct eval_action<parse::arrow_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.arrow ();
        }
    };

    template <> struct eval_action<parse::intuitionistic_and_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.intuitionistic_and ();
        }
    };

    template <> struct eval_action<parse::intuitionistic_or_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.intuitionistic_or ();
        }
    };

    template <> struct eval_action<parse::intuitionistic_implies_op> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.intuitionistic_implies ();
        }
    };

    template <> struct eval_action<parse::expression> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {}
    };

    template <> struct eval_action<parse::infer> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            eval.set ();
        }
    };

    template <> struct eval_action<parse::statement> {
        template <typename Input>
        static void apply (const Input& in, Diophant::evaluation &eval) {
            if (data::size (eval.Stack) == 1) {
                auto v = evaluate (eval.Stack.first (), eval.Vars);
                v->write (std::cout << "\n result: ") << std::endl;
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

    void inline evaluation::pow () {
        Stack = prepend (rest (rest (Stack)), expression::power (first (rest (Stack)), first (Stack)));
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
        if (v == nullptr) return v;

        return v->evaluate (vars);
    }

    struct boolean : expression {
        bool Value;
        boolean (const bool b) : Value {b} {}

        std::ostream &write (std::ostream &o) const override {
            return o << std::boolalpha << Value;
        }

        value operator ! () const override {
            return expression::boolean (!Value);
        }

        value operator && (value v) const override {
            auto r = std::dynamic_pointer_cast<const boolean> (v);
            if (r == nullptr) return this->shared_from_this ();
            return expression::boolean (Value && r->Value);
        }

        value operator || (value v) const override {
            auto r = std::dynamic_pointer_cast<const boolean> (v);
            if (r == nullptr) return this->shared_from_this ();
            return expression::boolean (Value || r->Value);
        }
    };

    struct symbol : expression {
        data::string Name;
        symbol (const data::string &x) : Name {x} {}

        std::ostream &write (std::ostream &o) const override {
            return o << Name;
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto x = vars.find (Name);
            if (x == vars.end ()) throw exception {} << "undefined symbol " << Name;

            return Diophant::evaluate (x->second, vars);
        };
    };

    struct string : expression {
        data::string Value;
        string (const data::string &x) : Value {x} {}

        std::ostream &write (std::ostream &o) const override {
            return o << "\"" << Value << "\"";
        }
    };

    struct rational : expression {
        data::Q Value;
        rational (const data::Q &q) : Value {q} {}

        std::ostream &write (std::ostream &o) const override {
            o << Value.Numerator;
            if (Value.Denominator != 1) o << "/" << Value.Denominator;
            return o;
        }

        value operator - () const override {
            return expression::rational (-Value);
        }

        value operator + (value v) const override {
            auto r = std::dynamic_pointer_cast<const rational> (v);
            if (r == nullptr) return this->shared_from_this ();
            return expression::rational (Value + r->Value);
        }

        value operator - (value v) const override {
            auto r = std::dynamic_pointer_cast<const rational> (v);
            if (r == nullptr) return this->shared_from_this ();
            return expression::rational (Value - r->Value);
        }

        value operator * (value v) const override {
            auto r = std::dynamic_pointer_cast<const rational> (v);
            if (r == nullptr) return this->shared_from_this ();
            return expression::rational (Value * r->Value);
        }

        value operator / (value v) const override {
            auto r = std::dynamic_pointer_cast<const rational> (v);
            if (r == nullptr) return this->shared_from_this ();
            return expression::rational (Value / math::nonzero<Q> (r->Value));
        }
    };

    struct list : expression {
        data::list<value> Value;
        list (data::list<value> v) : Value {v} {}

        std::ostream &write (std::ostream &o) const override {
            o << "[";

            if (!data::empty (Value)) {
                first (Value)->write (o);
                for (const auto v : rest (Value)) v->write (o << ", ");
            }

            return o << "]";
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            data::list<value> vvvvvv;
            for (value &v : Value) vvvvvv <<= Diophant::evaluate (v, vars);
            return expression::list (vvvvvv);
        };
    };

    struct object : expression {
        data::list<entry<data::string, value>> Value;
        object (data::list<entry<data::string, value>> v) : Value {v} {}

        std::ostream &write (std::ostream &o) const override {
            o << "{";

            if (!data::empty (Value)) {
                auto e = first (Value);
                e.Value->write (o << e.Key << ": ");
                for (const auto e : rest (Value)) e.Value->write (o << ", " << e.Key << ": ");
            }

            return o << "}";
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            return expression::object (data::for_each ([&vars] (entry<data::string, value> v) -> entry<data::string, value> {
                return entry<data::string, value> {v.Key, Diophant::evaluate (v.Value, vars)};
            }, Value));
        };
    };

    struct apply : expression {
        value Left;
        value Right;
        apply (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 100;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return (*a) (b);
        };
    };

    struct negate : expression {
        value Value;
        negate (const value &v) : Value {v} {}

        uint32 precedence () const override {
            return 200;
        }

        std::ostream &write (std::ostream &o) const override {
            o << "-";
            if (Value->precedence () > precedence ()) return Value->write (o << "(") << ")";
            else return Value->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto v = Diophant::evaluate (Value, vars);
            if (v.get () == nullptr) return expression::negate (v);
            return -(*v);
        };
    };

    struct boolean_not : expression {
        value Value;
        boolean_not (const value &v) : Value {v} {}

        uint32 precedence () const override {
            return 200;
        }

        std::ostream &write (std::ostream &o) const override {
            o << "!";
            if (Value->precedence () > precedence ()) return Value->write (o << "(") << ")";
            else return Value->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto v = Diophant::evaluate (Value, vars);
            if (v.get () == nullptr) return expression::negate (v);
            return !(*v);
        };
    };

    struct plus : expression {
        value Left;
        value Right;
        plus (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 300;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " + ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a + b;
        };
    };

    struct minus : expression {
        value Left;
        value Right;
        minus (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 400;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " - ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a - b;
        };
    };

    struct times : expression {
        value Left;
        value Right;
        times (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 500;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " * ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a * b;
        };
    };

    struct power : expression {
        value Left;
        value Right;
        power (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 550;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " ^ ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a ^ b;
        };
    };

    struct divide : expression {
        value Left;
        value Right;
        divide (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 600;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " / ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a / b;
        };
    };

    struct equal : expression {
        value Left;
        value Right;
        equal (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 700;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " == ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a == b;
        };
    };

    struct unequal : expression {
        value Left;
        value Right;
        unequal (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 700;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " != ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a != b;
        };
    };

    struct greater_equal : expression {
        value Left;
        value Right;
        greater_equal (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 700;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " >= ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a >= b;
        };
    };

    struct less_equal : expression {
        value Left;
        value Right;
        less_equal (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 700;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " <= ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a <= b;
        };
    };

    struct greater : expression {
        value Left;
        value Right;
        greater (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 700;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " > ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a > b;
        };
    };

    struct less : expression {
        value Left;
        value Right;
        less (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 700;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " < ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a < b;
        };
    };

    struct boolean_and : expression {
        value Left;
        value Right;
        boolean_and (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 800;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " && ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a && b;
        };
    };

    struct boolean_or : expression {
        value Left;
        value Right;
        boolean_or (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 900;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " || ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a || b;
        };
    };

    struct arrow : expression {
        value Left;
        value Right;
        arrow (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 1000;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " -> ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return a->arrow (b);
        };
    };

    struct intuitionistic_and : expression {
        value Left;
        value Right;
        intuitionistic_and (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 1100;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " & ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a & b;
        };
    };

    struct intuitionistic_or : expression {
        value Left;
        value Right;
        intuitionistic_or (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 1200;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " | ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return *a | b;
        };
    };

    struct intuitionistic_implies : expression {
        value Left;
        value Right;
        intuitionistic_implies (const value &a, const value &b) : Left {a}, Right {b} {}

        uint32 precedence () const override {
            return 1300;
        }

        std::ostream &write (std::ostream &o) const override {
            if (Left->precedence () > precedence ()) Left->write (o << "(") << ")";
            else Left->write (o);
            o << " => ";
            if (Right->precedence () > precedence ()) return Right->write (o << "(") << ")";
            else return Right->write (o);
        }

        value evaluate (const std::map<data::string, value> &vars) const override {
            auto a = Diophant::evaluate (Left, vars);
            auto b = Diophant::evaluate (Right, vars);
            if (a.get () == nullptr) return expression::apply (a, b);
            return a->implies (b);
        };
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

    value inline expression::list (const data::list<value> &ls) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::list> (ls));
    }

    value inline expression::object (const data::list<entry<data::string, value>> &x) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::object> (x));
    }

    value inline expression::apply (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::apply> (a, b));
    }

    value inline expression::operator () (const value x) const {
        return apply (this->shared_from_this (), x);
    }

    value inline expression::negate (const value x) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::negate> (x));
    }

    value inline expression::operator - () const {
        return negate (this->shared_from_this ());
    }

    value inline expression::plus (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::plus> (a, b));
    }

    value inline expression::operator + (const value v) const {
        return plus (this->shared_from_this (), v);
    }

    value inline expression::minus (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::minus> (a, b));
    }

    value inline expression::operator - (const value v) const {
        return minus (this->shared_from_this (), v);
    }

    value inline expression::times (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::times> (a, b));
    }

    value inline expression::operator * (const value v) const {
        return times (this->shared_from_this (), v);
    }

    value inline expression::divide (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::divide> (a, b));
    }

    value inline expression::operator / (const value v) const {
        return divide (this->shared_from_this (), v);
    }

    value inline expression::power (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::power> (a, b));
    }

    value inline expression::operator ^ (const value v) const {
        return power (this->shared_from_this (), v);
    }

    value inline expression::equal (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::equal> (a, b));
    }

    value inline expression::operator == (const value v) const {
        return equal (this->shared_from_this (), v);
    }

    value inline expression::unequal (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::unequal> (a, b));
    }

    value inline expression::operator != (const value v) const {
        return unequal (this->shared_from_this (), v);
    }

    value inline expression::greater_equal (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::greater_equal> (a, b));
    }

    value inline expression::operator >= (const value v) const {
        return greater_equal (this->shared_from_this (), v);
    }

    value inline expression::less_equal (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::less_equal> (a, b));
    }

    value inline expression::operator <= (const value v) const {
        return less_equal (this->shared_from_this (), v);
    }

    value inline expression::greater (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::greater> (a, b));
    }

    value inline expression::operator > (const value v) const {
        return greater (this->shared_from_this (), v);
    }

    value inline expression::less (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::less> (a, b));
    }

    value inline expression::operator < (const value v) const {
        return less (this->shared_from_this (), v);
    }

    value inline expression::boolean_not (const value x) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::boolean_not> (x));
    }

    value inline expression::operator ! () const {
        return boolean_not (this->shared_from_this ());
    }

    value inline expression::boolean_and (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::boolean_and> (a, b));
    }

    value inline expression::operator && (const value v) const {
        return boolean_and (this->shared_from_this (), v);
    }

    value inline expression::boolean_or (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::boolean_or> (a, b));
    }

    value inline expression::operator || (const value v) const {
        return boolean_or (this->shared_from_this (), v);
    }

    value inline expression::arrow (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::arrow> (a, b));
    }

    value inline expression::arrow (const value v) const {
        return arrow (this->shared_from_this (), v);
    }

    value inline expression::intuitionistic_and (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::intuitionistic_and> (a, b));
    }

    value inline expression::operator & (const value v) const {
        return intuitionistic_and (this->shared_from_this (), v);
    }

    value inline expression::intuitionistic_or (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::intuitionistic_or> (a, b));
    }

    value inline expression::operator | (const value v) const {
        return intuitionistic_or (this->shared_from_this (), v);
    }

    value inline expression::intuitionistic_implies (const value a, const value b) {
        return std::static_pointer_cast<expression> (std::make_shared<Diophant::intuitionistic_implies> (a, b));
    }

    value inline expression::implies (const value v) const {
        return intuitionistic_implies (this->shared_from_this (), v);
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
                tao::pegtl::parse<Diophant::parse::grammar, Diophant::eval_action> (input, eval);



            } catch (const std::exception& ex) {
                std::cerr << "Error: " << ex.what () << std::endl;
            }
        }

    }

}
