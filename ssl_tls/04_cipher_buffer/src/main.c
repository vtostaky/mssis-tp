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
    int output_len;
    int unciphered_len;

    const char input[450] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

    //const char input[450] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
    int input_len = strlen(input);

    if(cipher_buffer(&output, &output_len, (unsigned char*)input, input_len, "/home/vdrouin/tp/ssl_tls/rsa_pub.pem", "/home/vdrouin/tp/ssl_tls/rsa.pem") == 0)
    {
        print_hex(output, output_len, "Encrypted text");
        if(uncipher_buffer(&unciphered, &unciphered_len, output, output_len, "/home/vdrouin/tp/ssl_tls/rsa_pub.pem", "/home/vdrouin/tp/ssl_tls/rsa.pem") == 0)
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
