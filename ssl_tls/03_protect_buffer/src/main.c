#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "protect_buffer.h"
#include "tools.h"

int main(int argc, char **argv)
{
    int ret = -1;
    unsigned char master_key[HASH_SZ];
    unsigned char salt[SALT_SZ];

    unsigned char* output = NULL;
    unsigned char* unciphered = NULL;
    int output_len;
    int unciphered_len;

    const char *input = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

    int input_len = strlen(input);

    if(argc < 2)
    {
        printf("Usage : %s <passwd>\n",argv[0]);
        goto exit;
    }

    if(gen_alea(salt, SALT_SZ) != 0)
    {   
        printf("Could not generate salt!\n");
        goto exit;
    }

    if(deriv_passwd(master_key, argv[1], salt, SALT_SZ, MAX_ITER ) == 0)
    {
        print_hex(master_key, HASH_SZ, "master key");
        print_hex(salt, SALT_SZ, "salt");
        if(protect_buffer(&output, &output_len, (unsigned char*)input, input_len, master_key, HASH_SZ, salt, SALT_SZ) == 0)
        {
            print_hex(output, output_len, "Encrypted text");
            if(unprotect_buffer(&unciphered, &unciphered_len, (unsigned char*)output, output_len, master_key, HASH_SZ, SALT_SZ) == 0)
            {
                print_hex(unciphered, unciphered_len, "Decrypted text");
                print_char(unciphered, unciphered_len, "Decrypted text");
            }
        }
        else
        {
            printf("Could not encrypt buffer!\n");
            goto exit;
        }
    }
    else
    {
        printf("Could not derviate password!\n");
        goto exit;
    }
    ret = 0;
exit:
    if(output != NULL)
        secure_memzero(output, output_len);
    if(unciphered != NULL)
        secure_memzero(unciphered, unciphered_len);
    free(output);
    free(unciphered);
    return ret;
}
