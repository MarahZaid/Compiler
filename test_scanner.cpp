#include "fd.h"
#include "scanner.h"
#include <iostream>
using namespace std;

void printToken(Token* token) {
    if (!token) return;
    
    switch(token->type) {
        case T_EOF:
            cout << "EOF" << endl;
            break;
        case T_ID:
            cout << "IDENTIFIER: " << token->str_val << endl;
            break;
        case T_INTEGER:
            cout << "INTEGER: " << token->int_val << endl;
            break;
        case T_STRING:
            cout << "STRING: \"" << token->str_val << "\"" << endl;
            break;
        default:
            cout << "TOKEN type: " << token->type << endl;
            break;
    }
}

int main() {
    FileDescriptor fd("test_scanner.txt");
    
    if (!fd.IsOpen()) {
        cout << "Cannot open test_scanner.txt" << endl;
        return 1;
    }
    
    cout << "Testing Scanner..." << endl;
    cout << "==================" << endl;
    
    Scanner scanner;
    Token* token;
    int count = 0;
    
    while ((token = scanner.scan(&fd)) && token->type != T_EOF) {
        count++;
        cout << count << ": ";
        printToken(token);
        delete token;
    }
    
    cout << "==================" << endl;
    cout << "Total: " << count << " tokens" << endl;
    
    delete token;
    return 0;
}
