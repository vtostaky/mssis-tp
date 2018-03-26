#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main()
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
        return -1;
    }

    if(WriteFile(file, txt, strlen(txt), &dw, NULL))
        printf("Write %i bytes\n", dw);

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
        return -1;
    }

    ZeroMemory(txt_res, sizeof(txt_res));
    if(ReadFile(file, txt_res, sizeof(txt_res), &dw, NULL))
    {
        printf("Txt: %s\n", txt_res);
        printf("I  : %i", dw);
    }

    CloseHandle(file);
    
    return 0;
}
