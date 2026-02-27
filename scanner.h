#ifndef SCANNER_H
#define SCANNER_H

#include "fd.h"
#include <string>
#include <unordered_map>

using namespace std;

enum TokenType
{
    // Special
    T_EOF,
    T_ERROR,

    // Identifiers & literals
    T_ID,
    T_INTEGER,
    T_STRING,

    // Keywords
    T_PROGRAM,
    T_VAR,
    T_CONSTANT,
    T_INTEGER_KW,
    T_BOOL,
    T_STRING_KW,
    T_FLOAT,
    T_TRUE,
    T_FALSE,
    T_IF,
    T_FI,
    T_THEN,
    T_ELSE,
    T_WHILE,
    T_DO,
    T_OD,
    T_FOR,
    T_FROM,
    T_TO,
    T_BY,
    T_READ,
    T_WRITE,
    T_AND,
    T_OR,

    // Operators & symbols
    T_PLUS,        // +
    T_MINUS,       // -
    T_MULT,        // *
    T_DIV,         // /
    T_ASSIGN,      // :=
    T_EQUAL,       // =
    T_NOT_EQUAL,   // !=
    T_LT,          // <
    T_LE,          // <=
    T_GT,          // >
    T_GE,          // >=
    T_DOT,         // .
    T_SEMICOLON,   // ;
    T_COMMA,       // ,
    T_LPAREN,      // (
    T_RPAREN,      // )
    T_LBRACKET,    // [
    T_RBRACKET,    // ]
    T_LBRACE,      // {
    T_RBRACE       // }
};

struct Token
{
    TokenType type;
    string str_val;   // for ID and STRING
    int int_val;      // for INTEGER

    Token(TokenType t) : type(t), int_val(0) {}
};

class Scanner
{
private:
    unordered_map<string, TokenType> keywords;

    void InitKeywords();
    void SkipWhitespace(FileDescriptor *fd);
    void SkipComment(FileDescriptor *fd);
    Token* ScanIdentifierOrKeyword(FileDescriptor *fd, char first);
    Token* ScanNumber(FileDescriptor *fd, char first);
    Token* ScanString(FileDescriptor *fd);

public:
    Scanner();
    Token* scan(FileDescriptor *fd);
};

#endif
