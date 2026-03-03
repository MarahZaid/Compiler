#include "token_utils.h"

const char* token_to_string(LEXEME_TYPE type) {

    switch (type) {

        /* Literals */
        case lx_identifier: return "lx_identifier";
        case lx_integer: return "lx_integer";
        case lx_string: return "lx_string";
        case lx_float: return "lx_float";

        /* Keywords */
        case kw_program: return "kw_program";
        case kw_var: return "kw_var";
        case kw_constant: return "kw_constant";
        case kw_integer: return "kw_integer";
        case kw_bool: return "kw_bool";
        case kw_string: return "kw_string";
        case kw_float: return "kw_float";
        case kw_true: return "kw_true";
        case kw_false: return "kw_false";
        case kw_if: return "kw_if";
        case kw_fi: return "kw_fi";
        case kw_then: return "kw_then";
        case kw_else: return "kw_else";
        case kw_while: return "kw_while";
        case kw_do: return "kw_do";
        case kw_od: return "kw_od";
        case kw_and: return "kw_and";
        case kw_or: return "kw_or";
        case kw_read: return "kw_read";
        case kw_write: return "kw_write";
        case kw_for: return "kw_for";
        case kw_from: return "kw_from";
        case kw_to: return "kw_to";
        case kw_by: return "kw_by";
        case kw_function: return "kw_function";
        case kw_procedure: return "kw_procedure";
        case kw_return: return "kw_return";
        case kw_not: return "kw_not";
        case kw_begin: return "kw_begin";
        case kw_end: return "kw_end";

        /* Operators */
        case lx_lparen: return "lx_lparen";
        case lx_rparen: return "lx_rparen";
        case lx_lbracket: return "lx_lbracket";
        case lx_rbracket: return "lx_rbracket";
        case lx_lbrace: return "lx_lbrace";
        case lx_rbrace: return "lx_rbrace";
        case lx_colon: return "lx_colon";
        case lx_dot: return "lx_dot";
        case lx_semicolon: return "lx_semicolon";
        case lx_comma: return "lx_comma";
        case lx_colon_eq: return "lx_colon_eq";
        case lx_plus: return "lx_plus";
        case lx_minus: return "lx_minus";
        case lx_star: return "lx_star";
        case lx_slash: return "lx_slash";
        case lx_eq: return "lx_eq";
        case lx_neq: return "lx_neq";
        case lx_lt: return "lx_lt";
        case lx_le: return "lx_le";
        case lx_gt: return "lx_gt";
        case lx_ge: return "lx_ge";

        case lx_eof: return "lx_eof";

        default: return "UNKNOWN_TOKEN";
    }
}
