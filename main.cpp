#include <iostream>
#include "fd.h"
#include "scanner.h"
#include "token_utils.h"

using namespace std;

int main(int argc, char* argv[]) {

    cout << "COMPILER CONSTRUCTION - PA#1 Scanner" << endl;
    cout << "=====================================\n" << endl;

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    const char* filename = argv[1];

    FileDescriptor fd(filename);

    if (!fd.IsOpen()) {
        cout << "Failed to open file: " << filename << endl;
        return 1;
    }

    cout << "Reading File: " << fd.GetFileName() << endl;
    cout << "------------------------------------" << endl;

    SCANNER scanner(&fd);
    TOKEN* token = NULL;
    int tokenCount = 0;

    while (true) {

        token = scanner.Scan();

        if (!token)
            break;

        cout << ++tokenCount << ": "
             << token_to_string(token->type);

        if (token->type == lx_identifier && token->str_ptr)
            cout << " (" << token->str_ptr << ")";
        else if (token->type == lx_integer)
            cout << " (" << token->value << ")";
        else if (token->type == lx_float)
            cout << " (" << token->float_value << ")";
        else if (token->type == lx_string && token->str_ptr)
            cout << " (\"" << token->str_ptr << "\")";

        cout << endl;

        if (token->type == lx_eof) {
            delete token;
            break;
        }

        if (token->str_ptr)
            delete[] token->str_ptr;

        delete token;
    }

    cout << "------------------------------------" << endl;
    cout << "Total Tokens: " << tokenCount << endl;

    fd.Close();

    cout << "\nScanner finished successfully" << endl;

    return 0;
}
