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
    struct dec_lit : sor<one<'0'>, seq<range<'1', '9'>, star<digit>>> {};

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
    struct ws : star<space> {};

    struct expression;

    struct factor : sor<number_lit, string_lit, symbol, seq<open_paren, ws, expression, ws, close_paren>> {};

    struct unary_operator : sor<one<'~'>, one<'!'>, one<'+'>, one<'-'>, one<'*'>> {};

    struct comp_operator : sor<string<'<','='>, string<'>','='>, string<'=','='>, string<'!','='>, one<'>'>, one<'<'>> {};

    struct unary_expr;
    struct mul_expr;
    struct div_expr;
    struct sub_expr;
    struct add_expr;
    struct comp_expr;

    struct unary_op : seq<unary_operator, ws, unary_expr> {};

    struct mul_op : seq<ws, sor<one<'*'>, one<'%'>, one<'~'>>, ws, mul_expr> {};
    struct div_op : seq<ws, one<'/'>, ws, div_expr> {};
    struct sub_op : seq<ws, one<'-'>, ws, sub_expr> {};
    struct add_op : seq<ws, one<'+'>, ws, add_expr> {};
    struct comp_op : seq<ws, comp_operator, comp_expr> {};

    struct unary_expr : sor<unary_op, factor> {};

    struct mul_expr : seq<unary_expr, opt<mul_op>> {};
    struct div_expr : seq<mul_expr, opt<div_op>> {};
    struct sub_expr : seq<div_expr, opt<sub_op>> {};
    struct add_expr : seq<sub_expr, opt<add_op>> {};
    struct comp_expr : seq<add_expr, opt<comp_op>> {};

    struct and_expr : seq<comp_expr, opt<seq<ws, one<'^'>, ws, comp_expr>>> {};
    struct or_expr : seq<and_expr, opt<seq<ws, one<'|'>, ws, and_expr>>> {};
    struct expression : seq<or_expr, opt<seq<ws, string<'=','>'>, ws, or_expr>>> {};

    struct set : seq<one<'='>, ws, expression> {};
    struct infer : seq<string<':', '='>, ws, expression> {};
    struct declare : seq<one<':'>, ws, expression, opt<set>> {};

    struct statement : seq<expression, opt<ws, sor<infer, set, declare>>> {};

    struct grammar : seq<statement, ws> {};
}

#endif
