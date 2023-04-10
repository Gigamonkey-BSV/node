#ifndef NODE_CALC
#define NODE_CALC

#include <tao/pegtl.hpp>

namespace calc {
    using namespace tao::pegtl;

    // Define grammar rules for parsing arithmetic expressions
    struct number : plus<digit> {};
    struct symbol : seq<sor<alpha, one<'_'>>, star<sor<alnum, one<'_'>>>> {};

    struct open_paren : one<'('> {};
    struct close_paren : one<')'> {};
    struct whitespace : star<space> {};

    struct expression;

    struct factor : sor<number, symbol, seq<open_paren, whitespace, expression, whitespace, close_paren>> {};

    struct unary_op : sor<one<'+'>, one<'-'> > {};

    struct term;

    struct unary_expression : seq<opt<unary_op>, term> {};

    struct operation_mul_div : sor<one<'*'>, one<'/'>, one<'%'> > {};
    struct operation_add_sub : sor<one<'+'>, one<'-'> > {};

    struct term : list<unary_expression, operation_mul_div, whitespace> {};

    struct expression : list<term, operation_add_sub, whitespace> {};

    struct type_op : sor<one<'~'>, one<'|'>, one<'^'>, string<'=','>'> > {};

    struct type_expression : list<symbol, type_op, whitespace> {};

    struct statement : sor<
        seq<symbol, whitespace, one<'='>, whitespace, expression>,
        seq<symbol, whitespace, one<':'>, one<'='>, whitespace, expression>,
        seq<symbol, whitespace, one<':'>, whitespace, type_expression, whitespace, one<'='>, whitespace, expression>,
        seq<symbol, whitespace, one<':'>, whitespace, type_expression>,
        expression
    > {};

    struct grammar : must<expression, whitespace, one<';'>> {};
}

namespace Cosmos {
    void calc ();
}

#endif
