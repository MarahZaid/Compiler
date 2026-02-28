#include <iostream>
#include "fd.h"
#include "scanner.h"

using namespace std;

// PA#0 - FileDescriptor Test
void testFileDescriptor() {
    cout << "\n=== PA#0: Testing FileDescriptor ===" << endl;
    cout << "=====================================" << endl;
    
    FileDescriptor fd("test.txt");
    
    if (!fd.IsOpen()) {
        cout << "Failed to open test.txt!" << endl;
        return;
    }

    cout << "Reading file: " << fd.GetFileName() << endl;
    cout << "-----------------------------" << endl;

    int c;
    while ((c = fd.GetChar()) != EOF) {
        cout << (char)c;

        if (c == 'a') {
            cout << "\n(Unget applied)\n";
            fd.UngetChar(c);
            char again = fd.GetChar();
            cout << "Read again: " << again << endl;
        }

        if (c == '@') {
            fd.ReportError("Invalid symbol detected");
        }
    }

    fd.Close();
    cout << "\nFinished reading file." << endl;
}

// PA#1 - Scanner Test
void testScanner() {
    cout << "\n=== PA#1: Testing Scanner ===" << endl;
    cout << "==============================" << endl;
    
    FileDescriptor fd("test_scanner.txt");
    
    if (!fd.IsOpen()) {
        cout << "Failed to open test_scanner.txt!" << endl;
        return;
    }
    
    cout << "File: " << fd.GetFileName() << endl;
    cout << "------------------------" << endl;
    
    Scanner scanner;
    Token* token;
    int tokenCount = 0;
    
    while ((token = scanner.scan(&fd)) && token->type != T_EOF) {
        tokenCount++;
        cout << tokenCount << ": ";
        
        switch(token->type) {
            case T_ID:
                cout << "IDENTIFIER: " << token->str_val << endl;
                break;
            case T_INTEGER:
                cout << "INTEGER: " << token->int_val << endl;
                break;
            case T_STRING:
                cout << "STRING: \"" << token->str_val << "\"" << endl;
                break;
            case T_PLUS: cout << "OPERATOR: +" << endl; break;
            case T_MINUS: cout << "OPERATOR: -" << endl; break;
            case T_MULT: cout << "OPERATOR: *" << endl; break;
            case T_DIV: cout << "OPERATOR: /" << endl; break;
            case T_ASSIGN: cout << "OPERATOR: :=" << endl; break;
            case T_EQUAL: cout << "OPERATOR: =" << endl; break;
            case T_NOT_EQUAL: cout << "OPERATOR: !=" << endl; break;
            case T_LT: cout << "OPERATOR: <" << endl; break;
            case T_LE: cout << "OPERATOR: <=" << endl; break;
            case T_GT: cout << "OPERATOR: >" << endl; break;
            case T_GE: cout << "OPERATOR: >=" << endl; break;
            case T_SEMICOLON: cout << "OPERATOR: ;" << endl; break;
            case T_COMMA: cout << "OPERATOR: ," << endl; break;
            case T_LPAREN: cout << "OPERATOR: (" << endl; break;
            case T_RPAREN: cout << "OPERATOR: )" << endl; break;
            default:
                if (token->type >= T_PROGRAM && token->type <= T_OR)
                    cout << "KEYWORD" << endl;
                else
                    cout << "TOKEN(" << token->type << ")" << endl;
                break;
        }
        delete token;
    }
    
    cout << "------------------------" << endl;
    cout << "Total tokens: " << tokenCount << endl;
    delete token;
    fd.Close();
}

int main() {
    cout << "COMPILER CONSTRUCTION ASSIGNMENTS" << endl;
    cout << "==================================" << endl;
    
    // PA#0
    testFileDescriptor();
    
    cout << "\nPress Enter to continue to PA#1...";
    cin.get();
    
    // PA#1
    testScanner();
    
    cout << "\n\nAll assignments completed!" << endl;
    return 0;
}
