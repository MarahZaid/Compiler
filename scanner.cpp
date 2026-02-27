#include "scanner.h"
#include <cctype>
#include <cstdlib>

Scanner::Scanner()
{
    InitKeywords();
}

void Scanner::InitKeywords()
{
    keywords["program"] = T_PROGRAM;
    keywords["var"] = T_VAR;
    keywords["constant"] = T_CONSTANT;
    keywords["integer"] = T_INTEGER_KW;
    keywords["bool"] = T_BOOL;
    keywords["string"] = T_STRING_KW;
    keywords["float"] = T_FLOAT;
    keywords["true"] = T_TRUE;
    keywords["false"] = T_FALSE;
    keywords["if"] = T_IF;
    keywords["fi"] = T_FI;
    keywords["then"] = T_THEN;
    keywords["else"] = T_ELSE;
    keywords["while"] = T_WHILE;
    keywords["do"] = T_DO;
    keywords["od"] = T_OD;
    keywords["for"] = T_FOR;
    keywords["from"] = T_FROM;
    keywords["to"] = T_TO;
    keywords["by"] = T_BY;
    keywords["read"] = T_READ;
    keywords["write"] = T_WRITE;
    keywords["and"] = T_AND;
    keywords["or"] = T_OR;
}

void Scanner::SkipWhitespace(FileDescriptor *fd)
{
    int c;
    while ((c = fd->GetChar()) != EOF)
    {
        if (!isspace(c))
        {
            fd->UngetChar(c);
            return;
        }
    }
}

void Scanner::SkipComment(FileDescriptor *fd)
{
    int c = fd->GetChar();

    // ## ... ##
    if (c == '#')
    {
        int prev = 0;
        while ((c = fd->GetChar()) != EOF)
        {
            if (prev == '#' && c == '#')
                break;
            prev = c;
        }
    }
    else
    {
        // # until end of line
        while (c != '\n' && c != EOF)
            c = fd->GetChar();
    }
}

Token* Scanner::ScanIdentifierOrKeyword(FileDescriptor *fd, char first)
{
    string lexeme;
    lexeme += first;

    int c;
    while ((c = fd->GetChar()) != EOF)
    {
        if (isalnum(c) || c == '_')
            lexeme += c;
        else
        {
            fd->UngetChar(c);
            break;
        }
    }

    if (keywords.count(lexeme))
        return new Token(keywords[lexeme]);

    Token* tok = new Token(T_ID);
    tok->str_val = lexeme;
    return tok;
}

Token* Scanner::ScanNumber(FileDescriptor *fd, char first)
{
    string lexeme;
    lexeme += first;

    int c;
    while ((c = fd->GetChar()) != EOF)
    {
        if (isdigit(c))
            lexeme += c;
        else
        {
            fd->UngetChar(c);
            break;
        }
    }

    Token* tok = new Token(T_INTEGER);
    tok->int_val = atoi(lexeme.c_str());
    return tok;
}

Token* Scanner::ScanString(FileDescriptor *fd)
{
    string lexeme;
    int c;

    while ((c = fd->GetChar()) != EOF)
    {
        if (c == '"')
        {
            Token* tok = new Token(T_STRING);
            tok->str_val = lexeme;
            return tok;
        }

        if (c == '\n')
        {
            fd->ReportError("Unterminated string");
            return new Token(T_ERROR);
        }

        lexeme += c;
    }

    fd->ReportError("Unterminated string");
    return new Token(T_ERROR);
}

Token* Scanner::scan(FileDescriptor *fd)
{
    SkipWhitespace(fd);

    int c = fd->GetChar();

    if (c == EOF)
        return new Token(T_EOF);

    // Comment
    if (c == '#')
    {
        SkipComment(fd);
        return scan(fd);
    }

    // Identifier
    if (isalpha(c))
        return ScanIdentifierOrKeyword(fd, c);

    // Number or negative number
    if (isdigit(c) || (c == '-' && isdigit(fd->GetChar())))
    {
        if (c == '-')
        {
            fd->UngetChar(fd->GetChar());
            return ScanNumber(fd, c);
        }
        return ScanNumber(fd, c);
    }

    // String
    if (c == '"')
        return ScanString(fd);

    // Operators
    switch (c)
    {
    case '+': return new Token(T_PLUS);
    case '-': return new Token(T_MINUS);
    case '*': return new Token(T_MULT);
    case '/': return new Token(T_DIV);
    case '.': return new Token(T_DOT);
    case ';': return new Token(T_SEMICOLON);
    case ',': return new Token(T_COMMA);
    case '(': return new Token(T_LPAREN);
    case ')': return new Token(T_RPAREN);
    case '[': return new Token(T_LBRACKET);
    case ']': return new Token(T_RBRACKET);
    case '{': return new Token(T_LBRACE);
    case '}': return new Token(T_RBRACE);

    case ':':
        if (fd->GetChar() == '=')
            return new Token(T_ASSIGN);
        fd->ReportError("Invalid ':'");
        return new Token(T_ERROR);

    case '=':
        return new Token(T_EQUAL);

    case '!':
        if (fd->GetChar() == '=')
            return new Token(T_NOT_EQUAL);
        fd->ReportError("Invalid '!'");
        return new Token(T_ERROR);

    case '<':
        if (fd->GetChar() == '=')
            return new Token(T_LE);
        fd->UngetChar('=');
        return new Token(T_LT);

    case '>':
        if (fd->GetChar() == '=')
            return new Token(T_GE);
        fd->UngetChar('=');
        return new Token(T_GT);
    }

    fd->ReportError("Illegal character");
    return new Token(T_ERROR);
}
