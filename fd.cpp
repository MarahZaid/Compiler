#include "fd.h"

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

    // إذا السطر أطول من البافر
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

char FileDescriptor::GetChar()
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
    std::cerr << std::endl;   // ← هون ضيفها

    std::cerr << "Error in file: "
              << (file ? file : "stdin")
              << " at line " << line_number
              << " char " << char_number
              << " -> " << msg << std::endl;
}

void FileDescriptor::Close()
{
    if (fp && fp != stdin)
    {
        fclose(fp);
        fp = nullptr;
    }
}
