#include <stdio.h>
#include <stdlib.h>
#include "deriv_passwd.h"

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

int main(int argc, char **argv)
{
    int ret = -1;
    unsigned char output_key[32];
    unsigned char alea[32];
    int iterations;
   
    if(argc < 3)
    {
        printf("Usage : %s <passwd> <iterations>\n",argv[0]);
        goto exit;
    }
    else
    {
        iterations = atoi(argv[2]);
    }

    if(gen_alea(alea, 32) != 0)
    {   
        printf("Could not generate salt!\n");
        goto exit;
    }

    if(output_key != NULL)
    {
        if(deriv_passwd(output_key, argv[1], alea, 32, iterations ) == 0)
        {
            print_hex(output_key, 32, "key");
            print_hex(alea, 32, "salt");
        }
        else
        {
            printf("Could not generate key!\n");
            goto exit;
        }
        ret = 0;
    }
exit:
    return ret;
}
