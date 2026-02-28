// scanner.cpp
#include "scanner.h"
#include <cctype>
#include <cstdlib>
#include <iostream>

// Keyword list (must be kept in alphabetical order for binary search)
const int keys = 30; /* number of keywords */
const char *keyword[] = {
    "and", "begin", "boolean", "by", "constant",
    "do", "else", "end", "false", "fi", "float", "for", "from",
    "function", "if", "integer", "not", "od", "or", "procedure",
    "program", "read", "return", "string", "then", "to", "true",
    "var", "while", "write"
};

const LEXEME_TYPE key_type[] = {
    kw_and, kw_begin, kw_boolean, kw_by, kw_constant,
    kw_do, kw_else, kw_end, kw_false, kw_fi, kw_float,
    kw_for, kw_from, kw_function, kw_if, kw_integer, kw_not,
    kw_od, kw_or, kw_procedure, kw_program, kw_read, kw_return,
    kw_string, kw_then, kw_to, kw_true, kw_var, kw_while, kw_write
};

SCANNER::SCANNER() {
    Fd = nullptr;
    current_char = ' ';
}

SCANNER::SCANNER(FileDescriptor *fd) {
    Fd = fd;
    current_char = ' ';
}

SCANNER::~SCANNER() {
    // Nothing to clean up
}

void SCANNER::next_char() {
    if (Fd) {
        current_char = Fd->GetChar();
    } else {
        current_char = EOF;
    }
}

void SCANNER::skip_comments() {
    if (current_char == '#') {
        next_char();
        if (current_char == '#') {
            // ## comment - skip entire line
            while (current_char != '\n' && current_char != EOF) {
                next_char();
            }
        } else {
            // # comment - skip to end of line
            while (current_char != '\n' && current_char != EOF) {
                next_char();
            }
        }
        // Skip whitespace after comment
        while (isspace(current_char)) {
            next_char();
        }
    }
}

bool SCANNER::check_keyword(const char *str, LEXEME_TYPE &type) {
    // Binary search in keyword list
    int low = 0;
    int high = keys - 1;
    
    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(str, keyword[mid]);
        
        if (cmp == 0) {
            type = key_type[mid];
            return true;
        } else if (cmp < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    
    return false;
}

TOKEN* SCANNER::get_identifier() {
    TOKEN *token = new TOKEN;
    char buffer[256];
    int pos = 0;
    
    while (isalnum(current_char) || current_char == '_') {
        buffer[pos++] = current_char;
        next_char();
    }
    buffer[pos] = '\0';
    
    // Check if it's a keyword
    LEXEME_TYPE kw_type;
    if (check_keyword(buffer, kw_type)) {
        token->type = kw_type;
        token->str_ptr = nullptr;
        token->value = 0;
    } else {
        token->type = lx_identifier;
        token->str_ptr = new char[pos + 1];
        strcpy(token->str_ptr, buffer);
        token->value = 0; // Could store hash value here
    }
    
    return token;
}

TOKEN* SCANNER::get_string() {
    TOKEN *token = new TOKEN;
    token->type = lx_string;
    
    // Skip opening quote
    next_char();
    
    char *buffer = new char[256];
    int pos = 0;
    int size = 256;
    
    while (current_char != '"' && current_char != '\n' && current_char != EOF) {
        if (pos >= size - 1) {
            // Double buffer size
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
        next_char(); // Skip closing quote
    } else {
        Fd->ReportError("Unterminated string");
        token->str_ptr = nullptr;
    }
    
    delete[] buffer;
    return token;
}

TOKEN* SCANNER::get_number() {
    TOKEN *token = new TOKEN;
    char buffer[256];
    int pos = 0;
    bool is_float = false;
    
    if (current_char == '-') {
        buffer[pos++] = current_char;
        next_char();
    }
    
    while (isdigit(current_char)) {
        buffer[pos++] = current_char;
        next_char();
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
    } else {
        token->type = lx_integer;
        token->value = atoi(buffer);
        token->float_value = 0.0;
    }
    
    return token;
}

TOKEN* SCANNER::get_operator() {
    TOKEN *token = new TOKEN;
    char c = current_char;
    next_char();
    
    switch (c) {
        case '(': token->type = lx_lparen; break;
        case ')': token->type = lx_rparen; break;
        case '[': token->type = lx_lbracket; break;
        case ']': token->type = lx_rbracket; break;
        case ':': 
            if (current_char == '=') {
                token->type = lx_colon_eq;
                next_char();
            } else {
                token->type = lx_colon;
            }
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
            } else {
                Fd->ReportError("Invalid operator");
                token->type = lx_eof;
            }
            break;
        case '<':
            if (current_char == '=') {
                token->type = lx_le;
                next_char();
            } else {
                token->type = lx_lt;
            }
            break;
        case '>':
            if (current_char == '=') {
                token->type = lx_ge;
                next_char();
            } else {
                token->type = lx_gt;
            }
            break;
        default:
            Fd->ReportError("Invalid character");
            token->type = lx_eof;
    }
    
    return token;
}

TOKEN* SCANNER::Scan() {
    if (!Fd || !Fd->IsOpen()) {
        TOKEN *eof_token = new TOKEN;
        eof_token->type = lx_eof;
        return eof_token;
    }
    
    // Skip whitespace
    while (isspace(current_char)) {
        next_char();
    }
    
    // Skip comments
    skip_comments();
    
    // Check for EOF
    if (current_char == EOF) {
        TOKEN *eof_token = new TOKEN;
        eof_token->type = lx_eof;
        return eof_token;
    }
    
    // Process based on current character
    if (isalpha(current_char) || current_char == '_') {
        return get_identifier();
    } else if (isdigit(current_char) || (current_char == '-' && isdigit(Fd->GetChar()))) {
        return get_number();
    } else if (current_char == '"') {
        return get_string();
    } else {
        return get_operator();
    }
}
