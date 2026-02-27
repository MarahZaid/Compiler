#include <iostream>
#include "fd.h"

using namespace std;

int main()
{

    FileDescriptor fd("C:/Users/Admin/OneDrive/Desktop/compiler/compiler/test.txt");

    if (!fd.IsOpen())
    {
        cout << "Failed to open file!" << endl;
        return 1;
    }

    cout << "Reading file: " << fd.GetFileName() << endl;
    cout << "-----------------------------" << endl;

    char c;

    while ((c = fd.GetChar()) != EOF)
    {
        cout << c;

        // مثال تجربة Unget
        if (c == 'a')
        {
            cout << "\n(Unget applied)\n";
            fd.UngetChar(c);
            char again = fd.GetChar();
            cout << "Read again: " << again << endl;
        }

        // مثال تجربة ReportError
        if (c == '@')
        {
            fd.ReportError("Invalid symbol detected");
        }
    }

    fd.Close();

    cout << "\n\nFinished reading file." << endl;

    return 0;
}
