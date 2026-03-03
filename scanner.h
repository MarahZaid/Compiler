// scanner.h
#ifndef __SCANNER__H
#define __SCANNER__H

#include "fd.h"

// Token types enumeration
typedef enum {

    /* Literals */
    lx_identifier,
    lx_integer,
    lx_string,
    lx_float,

    /* Keywords */
    kw_program,
    kw_var,
    kw_constant,
    kw_integer,
    kw_bool,
    kw_string,
    kw_float,
    kw_true,
    kw_false,
    kw_if,
    kw_fi,
    kw_then,
    kw_else,
    kw_while,
    kw_do,
    kw_od,
    kw_and,
    kw_or,
    kw_read,
    kw_write,
    kw_for,
    kw_from,
    kw_to,
    kw_by,
    kw_function,
    kw_procedure,
    kw_return,
    kw_not,
    kw_begin,
    kw_end,

    /* Operators */
    lx_lparen,     // (
    lx_rparen,     // )
    lx_lbracket,   // [
    lx_rbracket,   // ]
    lx_lbrace,     // {
    lx_rbrace,     // }
    lx_colon,      // :
    lx_dot,        // .
    lx_semicolon,  // ;
    lx_comma,      // ,
    lx_colon_eq,   // :=
    lx_plus,       // +
    lx_minus,      // -
    lx_star,       // *
    lx_slash,      // /
    lx_eq,         // =
    lx_neq,        // !=
    lx_lt,         // <
    lx_le,         // <=
    lx_gt,         // >
    lx_ge,         // >=

    lx_eof

} LEXEME_TYPE;


// TOKEN definition
struct TOKEN {
    LEXEME_TYPE type;
    int value;
    float float_value;
    char *str_ptr;
};


class SCANNER {

private:
    FileDescriptor *Fd;
    char current_char;

    void next_char();
    void skip_whitespace_and_comments();
    bool check_keyword(const char *str, LEXEME_TYPE &type);

    TOKEN* get_identifier();
    TOKEN* get_string();
    TOKEN* get_number();
    TOKEN* get_operator();

public:
    SCANNER();
    SCANNER(FileDescriptor *fd);
    ~SCANNER();

    TOKEN* Scan();
};

#endif
