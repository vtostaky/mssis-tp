#include <stdio.h>
#include <stdlib.h>
#include "tools.h"


void print_hex(unsigned char* buffer, int length, const char* name)
{
    int i;

    if(buffer == NULL || name == NULL)
        return;

    printf("%s (%d) = ", name, length);
    for(i = 0; i < length; i++)
        printf("%02x", buffer[i]);
    printf("\n");
}

void print_char(unsigned char* buffer, int length, const char* name)
{
    int i;

    if(buffer == NULL || name == NULL)
        return;

    printf("%s (%d) = ", name, length);
    for(i = 0; i < length; i++)
        printf("%c", buffer[i]);
    printf("\n");
}
