#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void openWriteAndReadFile()
{
    DWORD dw;
    char txt[] = "Ceci est un essai";
    char txt_res[20];

    HANDLE file = CreateFile("test.txt",
            GENERIC_ALL,
            0, NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
    if(file == INVALID_HANDLE_VALUE)
    {
        printf("Can't create file!\n");
        return;
    }

    if(WriteFile(file, txt, strlen(txt), &dw, NULL))
        printf("Write %ld bytes\n", dw);

    CloseHandle(file);

    file = CreateFile("test.txt",
            GENERIC_ALL,
            0,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

    if(file == INVALID_HANDLE_VALUE)
    {
        printf("Can't open file for reading!\n");
        return;
    }

    ZeroMemory(txt_res, sizeof(txt_res));
    if(ReadFile(file, txt_res, sizeof(txt_res), &dw, NULL))
    {
        printf("Txt: %s\n", txt_res);
        printf("I  : %ld", dw);
    }

    CloseHandle(file);
}

void openDeviceFile()
{
    HANDLE file;
    file = CreateFile("\\\\.\\vince",
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);
    if(file == INVALID_HANDLE_VALUE)
    {
        printf("Can't open file for reading : Error 0x%x!\n", (UINT)GetLastError());
        return;
    }
    CloseHandle(file);
}
int main()
{
    openDeviceFile();
    return 0;
}
