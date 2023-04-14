#ifndef NODE_CALC
#define NODE_CALC

#include <tao/pegtl.hpp>
#include "types.hpp"

namespace Cosmos {
    // running calculator program.
    void calc ();
}

namespace calc {
    using namespace tao::pegtl;

    struct hex_digit : seq<xdigit, xdigit> {};
    struct hex_lit : seq<string<'0', 'x'>, star<hex_digit>> {};
    struct dec_lit : seq<range<'1', '9'>, star<digit>> {};

    struct number_lit : sor<dec_lit, hex_lit> {};

    struct string_lit : seq<
        one<'"'>,                                                     // opening "
            star<sor<
                seq<not_at<sor<one<'\\'>, one<'"'>>>, ascii::print>,
                seq<one<'\\'>, ascii::print>
            >>,
        one<'"'>> {};                                                 // closing "

    struct symbol : seq<sor<alpha, one<'_'>>, star<sor<alnum, one<'_'>>>> {};

    struct open_paren : one<'('> {};
    struct close_paren : one<')'> {};
    struct whitespace : star<space> {};

    struct expression;

    struct factor : sor<number_lit, string_lit, symbol, seq<open_paren, whitespace, expression, whitespace, close_paren>> {};

    struct unary_operator : sor<one<'~'>, one<'!'>, one<'+'>, one<'-'>, one<'*'>> {};

    struct comp_operator : sor<string<'<','='>, string<'>','='>, string<'=','='>, string<'!','='>, one<'>'>, one<'<'>> {};

    struct unary_expr : sor<factor, whitespace, seq<unary_operator, whitespace, unary_operator>> {};

    struct mul_expr;
    struct div_expr;
    struct sub_expr;
    struct add_expr;
    struct comp_expr;

    struct mul_op : seq<whitespace, sor<one<'*'>, one<'%'>, one<'~'>>, whitespace, mul_expr> {};
    struct div_op : seq<whitespace, one<'/'>, whitespace, div_expr> {};
    struct sub_op : seq<whitespace, one<'-'>, whitespace, sub_expr> {};
    struct add_op : seq<whitespace, one<'+'>, whitespace, add_expr> {};
    struct comp_op : seq<whitespace, comp_operator, comp_expr> {};

    struct mul_expr : seq<unary_expr, opt<mul_op>> {};
    struct div_expr : seq<mul_expr, opt<div_op>> {};
    struct sub_expr : seq<div_expr, opt<sub_op>> {};
    struct add_expr : seq<sub_expr, opt<add_op>> {};
    struct comp_expr : seq<add_expr, opt<comp_op>> {};

    struct and_expr : seq<comp_expr, opt<seq<whitespace, one<'^'>, whitespace, comp_expr>>> {};
    struct or_expr : seq<and_expr, opt<seq<whitespace, one<'|'>, whitespace, and_expr>>> {};
    struct expression : seq<or_expr, opt<seq<whitespace, string<'=','>'>, whitespace, or_expr>>> {};

    struct statement : sor<
        seq<symbol, whitespace, one<'='>, whitespace, expression>,
        seq<symbol, whitespace, one<':'>, one<'='>, whitespace, expression>,
        seq<symbol, whitespace, one<':'>, whitespace, expression, whitespace, one<'='>, whitespace, expression>,
        seq<symbol, whitespace, one<':'>, whitespace, expression>,
        expression
    > {};

    struct grammar : must<statement, whitespace, eof> {};
}

#endif
