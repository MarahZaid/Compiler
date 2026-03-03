#include "scanner.h"
#include <cctype>
#include <cstring>
#include <cstdlib>

/* ================= KEYWORDS ================= */

const int keys = 30;

const char *keyword[] = {
    "and", "begin", "bool", "by", "constant",
    "do", "else", "end", "false", "fi", "float", "for", "from",
    "function", "if", "integer", "not", "od", "or", "procedure",
    "program", "read", "return", "string", "then", "to", "true",
    "var", "while", "write"
};

const LEXEME_TYPE key_type[] = {
    kw_and, kw_begin, kw_bool, kw_by, kw_constant,
    kw_do, kw_else, kw_end, kw_false, kw_fi, kw_float,
    kw_for, kw_from, kw_function, kw_if, kw_integer, kw_not,
    kw_od, kw_or, kw_procedure, kw_program, kw_read, kw_return,
    kw_string, kw_then, kw_to, kw_true, kw_var, kw_while, kw_write
};


/* ================= CONSTRUCTORS ================= */

SCANNER::SCANNER() {
    Fd = NULL;
    current_char = ' ';
}

SCANNER::SCANNER(FileDescriptor *fd) {
    Fd = fd;
    current_char = ' ';
}

SCANNER::~SCANNER() {}


/* ================= BASIC HELPERS ================= */

void SCANNER::next_char() {
    if (Fd && Fd->IsOpen())
        current_char = Fd->GetChar();
    else
        current_char = EOF;
}


void SCANNER::skip_whitespace_and_comments() {

    while (true) {

        while (isspace(current_char))
            next_char();

        if (current_char == '#') {
            while (current_char != '\n' && current_char != EOF)
                next_char();
        }
        else
            break;
    }
}


bool SCANNER::check_keyword(const char *str, LEXEME_TYPE &type) {

    int low = 0;
    int high = keys - 1;

    while (low <= high) {

        int mid = (low + high) / 2;
        int cmp = strcmp(str, keyword[mid]);

        if (cmp == 0) {
            type = key_type[mid];
            return true;
        }
        else if (cmp < 0)
            high = mid - 1;
        else
            low = mid + 1;
    }

    return false;
}


/* ================= IDENTIFIER ================= */

TOKEN* SCANNER::get_identifier() {

    TOKEN *token = new TOKEN;
    char buffer[256];
    int pos = 0;

    while (isalnum(current_char) || current_char == '_') {
        buffer[pos++] = current_char;
        next_char();
    }

    buffer[pos] = '\0';

    LEXEME_TYPE kw_type;

    if (check_keyword(buffer, kw_type)) {
        token->type = kw_type;
        token->str_ptr = NULL;
    }
    else {
        token->type = lx_identifier;
        token->str_ptr = new char[pos + 1];
        strcpy(token->str_ptr, buffer);
    }

    token->value = 0;
    token->float_value = 0.0;

    return token;
}


/* ================= STRING ================= */

TOKEN* SCANNER::get_string() {

    TOKEN *token = new TOKEN;
    token->type = lx_string;

    next_char(); // skip "

    char *buffer = new char[256];
    int size = 256;
    int pos = 0;

    while (current_char != '"' &&
           current_char != '\n' &&
           current_char != EOF) {

        if (pos >= size - 1) {
            char *new_buffer = new char[size * 2];
            memcpy(new_buffer, buffer, size);
            delete[] buffer;
            buffer = new_buffer;
            size *= 2;
        }

        buffer[pos++] = current_char;
        next_char();
    }

    if (current_char == '"') {
        buffer[pos] = '\0';
        token->str_ptr = new char[pos + 1];
        strcpy(token->str_ptr, buffer);
        next_char();
    }
    else {
        Fd->ReportError("Unterminated string");
        token->str_ptr = NULL;
    }

    delete[] buffer;

    token->value = 0;
    token->float_value = 0.0;

    return token;
}


/* ================= NUMBER ================= */

TOKEN* SCANNER::get_number() {

    TOKEN *token = new TOKEN;
    char buffer[256];
    int pos = 0;
    bool is_float = false;

    while (isdigit(current_char)) {
        buffer[pos++] = current_char;
        next_char();
    }

    /* 🔥 FIX 1: منع 34xyz */
    if (isalpha(current_char) || current_char == '_') {

        while (isalnum(current_char) || current_char == '_')
            next_char();

        Fd->ReportError("Invalid identifier starting with digit");

        delete token;
        return Scan();   // كمل قراءة
    }

    if (current_char == '.') {
        is_float = true;
        buffer[pos++] = current_char;
        next_char();

        while (isdigit(current_char)) {
            buffer[pos++] = current_char;
            next_char();
        }
    }

    buffer[pos] = '\0';

    if (is_float) {
        token->type = lx_float;
        token->float_value = atof(buffer);
        token->value = 0;
    }
    else {
        token->type = lx_integer;
        token->value = atoi(buffer);
        token->float_value = 0.0;
    }

    token->str_ptr = NULL;
    return token;
}


/* ================= OPERATOR ================= */

TOKEN* SCANNER::get_operator() {

    TOKEN *token = new TOKEN;
    char c = current_char;
    next_char();

    switch (c) {

        case '(': token->type = lx_lparen; break;
        case ')': token->type = lx_rparen; break;
        case '[': token->type = lx_lbracket; break;
        case ']': token->type = lx_rbracket; break;
        case '{': token->type = lx_lbrace; break;
        case '}': token->type = lx_rbrace; break;

        case ':':
            if (current_char == '=') {
                token->type = lx_colon_eq;
                next_char();
            }
            else token->type = lx_colon;
            break;

        case '.': token->type = lx_dot; break;
        case ';': token->type = lx_semicolon; break;
        case ',': token->type = lx_comma; break;
        case '+': token->type = lx_plus; break;
        case '-': token->type = lx_minus; break;
        case '*': token->type = lx_star; break;
        case '/': token->type = lx_slash; break;
        case '=': token->type = lx_eq; break;

        case '!':
            if (current_char == '=') {
                token->type = lx_neq;
                next_char();
            }
            else {
                Fd->ReportError("Invalid operator");
                delete token;
                return Scan();
            }
            break;

        case '<':
            if (current_char == '=') {
                token->type = lx_le;
                next_char();
            }
            else token->type = lx_lt;
            break;

        case '>':
            if (current_char == '=') {
                token->type = lx_ge;
                next_char();
            }
            else token->type = lx_gt;
            break;

        default:
            /* 🔥 FIX 2: illegal char ما يوقف البرنامج */
            Fd->ReportError("Illegal character");
            delete token;
            return Scan();
    }

    token->value = 0;
    token->float_value = 0.0;
    token->str_ptr = NULL;

    return token;
}


/* ================= MAIN SCAN ================= */

TOKEN* SCANNER::Scan() {

    if (!Fd || !Fd->IsOpen()) {
        TOKEN *eof_token = new TOKEN;
        eof_token->type = lx_eof;
        eof_token->str_ptr = NULL;
        return eof_token;
    }

    if (current_char == ' ')
        next_char();

    skip_whitespace_and_comments();

    if (current_char == EOF) {
        TOKEN *eof_token = new TOKEN;
        eof_token->type = lx_eof;
        eof_token->str_ptr = NULL;
        return eof_token;
    }

    if (isalpha(current_char) || current_char == '_')
        return get_identifier();

    if (isdigit(current_char))
        return get_number();

    if (current_char == '"')
        return get_string();

    return get_operator();
}
