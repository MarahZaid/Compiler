#include <iostream>
#include "fd.h"
#include "scanner.h"

using namespace std;

/* ============================= */
/*        PA#0 - File Test       */
/* ============================= */

void testFileDescriptor() {
    cout << "\n=== PA#0: Testing FileDescriptor ===" << endl;
    cout << "=====================================" << endl;

    FileDescriptor fd("C:/Users/HP/Compiler/test.txt");

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


/* ============================= */
/*        PA#1 - Scanner Test    */
/* ============================= */

void testScanner() {

    cout << "\n=== PA#1: Testing Scanner ===" << endl;
    cout << "==============================" << endl;

    FileDescriptor fd("C:/Users/HP/Compiler/test_scanner.txt");

    if (!fd.IsOpen()) {
        cout << "Failed to open test_scanner.txt!" << endl;
        return;
    }

    cout << "File: " << fd.GetFileName() << endl;
    cout << "------------------------" << endl;

    SCANNER scanner(&fd);
    TOKEN* token = NULL;
    int tokenCount = 0;

    while (true) {

        token = scanner.Scan();

        if (!token)
            break;

        if (token->type == lx_eof) {
            delete token;
            break;
        }

        tokenCount++;
        cout << tokenCount << ": ";

        switch (token->type) {

            case lx_identifier:
                cout << "IDENTIFIER: " << token->str_ptr << endl;
                break;

            case lx_integer:
                cout << "INTEGER: " << token->value << endl;
                break;

            case lx_float:
                cout << "FLOAT: " << token->float_value << endl;
                break;

            case lx_string:
                cout << "STRING: \"" << token->str_ptr << "\"" << endl;
                break;

            case lx_plus:        cout << "OPERATOR: +" << endl; break;
            case lx_minus:       cout << "OPERATOR: -" << endl; break;
            case lx_star:        cout << "OPERATOR: *" << endl; break;
            case lx_slash:       cout << "OPERATOR: /" << endl; break;
            case lx_colon_eq:    cout << "OPERATOR: :=" << endl; break;
            case lx_eq:          cout << "OPERATOR: =" << endl; break;
            case lx_neq:         cout << "OPERATOR: !=" << endl; break;
            case lx_lt:          cout << "OPERATOR: <" << endl; break;
            case lx_le:          cout << "OPERATOR: <=" << endl; break;
            case lx_gt:          cout << "OPERATOR: >" << endl; break;
            case lx_ge:          cout << "OPERATOR: >=" << endl; break;
            case lx_semicolon:   cout << "OPERATOR: ;" << endl; break;
            case lx_comma:       cout << "OPERATOR: ," << endl; break;
            case lx_lparen:      cout << "OPERATOR: (" << endl; break;
            case lx_rparen:      cout << "OPERATOR: )" << endl; break;

            default:
                cout << "TOKEN TYPE: " << token->type << endl;
                break;
        }

        // تنظيف الذاكرة
        if (token->str_ptr)
            delete[] token->str_ptr;

        delete token;
    }

    cout << "------------------------" << endl;
    cout << "Total tokens: " << tokenCount << endl;

    fd.Close();
}


/* ============================= */
/*              MAIN             */
/* ============================= */

int main() {

    cout << "COMPILER CONSTRUCTION ASSIGNMENTS" << endl;
    cout << "==================================" << endl;

    testFileDescriptor();

    cout << "\nPress Enter to continue to PA#1...";
    cin.get();

    testScanner();

    cout << "\n\nAll assignments completed!" << endl;

    return 0;
}
