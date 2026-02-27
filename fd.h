#ifndef FD_H
#define FD_H

#include <cstdio>
#include <cstring>
#include <iostream>

#define SET 1
#define UNSET 0
#define BUFFER_SIZE 256

class FileDescriptor
{
private:
    FILE *fp;
    int line_number;
    int char_number;
    int flag;
    int buf_size;
    char *buffer;
    char *file;

    bool ReadNextLine();

public:
    FileDescriptor();
    FileDescriptor(const char *FileName);
    ~FileDescriptor();

    bool IsOpen();
    char *GetFileName();
    char *GetCurrLine();
    int GetLineNum();
    int GetCharNum();

    void Close();
    char GetChar();
    void UngetChar(char c);
    void ReportError(const char *msg);
};

#endif
