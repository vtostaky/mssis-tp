#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cipher_buffer.h"
#include "tools.h"

int main(int argc, char **argv)
{
    int ret = -1;

    unsigned char* output = NULL;
    unsigned char* unciphered = NULL;
    char* sign_priv = NULL;
    char* sign_pub = NULL;
    char* cipher_priv = NULL;
    char* cipher_pub = NULL;
    int output_len;
    int unciphered_len;

    const char input[450] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

    int input_len = strlen(input);

    if(argc < 5)
    {
        printf("Usage :\n./cipher_buffer <my_sign_priv.pem> <my_sign_pub.pem> <my_ciph_priv.pem> <my_ciph_pub.pem>\n");
        goto exit;
    }
    else
    {
        sign_priv = argv[1];
        sign_pub = argv[2];
        cipher_priv = argv[3];
        cipher_pub = argv[4];

        if(cipher_buffer(&output, &output_len, (unsigned char*)input, input_len, cipher_pub, sign_priv) == 0)
        {
            print_hex(output, output_len, "Encrypted text");
            if(uncipher_buffer(&unciphered, &unciphered_len, output, output_len, sign_pub, cipher_priv) == 0)
            {
                print_hex(unciphered, unciphered_len, "Decrypted text");
                print_char(unciphered, unciphered_len, "Decrypted text");
            }
            else
            {
                printf("Could not decrypt buffer!\n");
                goto exit;
            }
        }
        else
        {
            printf("Could not encrypt buffer!\n");
            goto exit;
        }
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
