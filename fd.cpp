#include "fd.h"
using namespace std;
FileDescriptor::FileDescriptor()
{
    fp = stdin;
    line_number = 0;
    char_number = 0;
    flag = UNSET;

    buf_size = BUFFER_SIZE;
    buffer = new char[buf_size];
    buffer[0] = '\0';

    file = nullptr;
}

FileDescriptor::FileDescriptor(const char *FileName)
{
    fp = fopen(FileName, "r");

    line_number = 0;
    char_number = 0;
    flag = UNSET;

    buf_size = BUFFER_SIZE;
    buffer = new char[buf_size];
    buffer[0] = '\0';

    if (FileName)
    {
        file = new char[strlen(FileName) + 1];
        strcpy(file, FileName);
    }
    else
        file = nullptr;
}

FileDescriptor::~FileDescriptor()
{
    Close();

    if (buffer)
        delete[] buffer;

    if (file)
        delete[] file;
}

bool FileDescriptor::IsOpen()
{
    return (fp != nullptr);
}

char *FileDescriptor::GetFileName()
{
    return file;
}

char *FileDescriptor::GetCurrLine()
{
    return buffer;
}

int FileDescriptor::GetLineNum()
{
    return line_number;
}

int FileDescriptor::GetCharNum()
{
    return char_number;
}

bool FileDescriptor::ReadNextLine()
{
    if (!fp)
        return false;

    char_number = 0;

    if (!fgets(buffer, buf_size, fp))
        return false;


    while (strchr(buffer, '\n') == nullptr && !feof(fp))
    {
        int old_size = buf_size;
        buf_size *= 2;

        char *newBuffer = new char[buf_size];

        strcpy(newBuffer, buffer);

        delete[] buffer;
        buffer = newBuffer;

        fgets(buffer + strlen(buffer), buf_size - strlen(buffer), fp);
    }

    line_number++;
    return true;
}

int FileDescriptor::GetChar()
{
    if (buffer[char_number] == '\0')
    {
        if (!ReadNextLine())
            return EOF;
    }

    flag = UNSET;
    return buffer[char_number++];
}

void FileDescriptor::UngetChar(char c)
{
    if (flag == SET || char_number <= 0)
        return;

    char_number--;
    flag = SET;
}
void FileDescriptor::ReportError(const char *msg)
{
    cout << "\n";

    // اطبع السطر الحالي
    cout << buffer;

    // اطبع مسافات لحد مكان الخطأ
    int pos = (char_number > 0) ? char_number - 1 : 0;

    for (int i = 0; i < pos; i++)
    {
        cout << (buffer[i] == '\t' ? '\t' : ' ');
    }

    cout << "^\n";

    // اطبع رسالة الخطأ بالشكل المطلوب
    cout << "Error: \"" << msg << "\" on line "
         << line_number
         << " of "
         << (file ? file : "stdin")
         << "\n\n";
}
void FileDescriptor::Close()
{
    if (fp && fp != stdin)
    {
        fclose(fp);
        fp = nullptr;
    }
}
