#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cipher_multi_users.h"
#include "tools.h"

int main(int argc, char **argv)
{
    int ret = -1;

    char *input = "/home/vdrouin/tp/ssl_tls/vincentdrouin.pdf";
    char *output = "/home/vdrouin/tp/ssl_tls/ciphered";
    char *rsa_pub = "/home/vdrouin/tp/ssl_tls/rsa_pub.pem";
    char *rsa_priv = "/home/vdrouin/tp/ssl_tls/rsa.pem";

    if(cipher_buffer(input, output, rsa_priv, &rsa_pub, 1) == 0)
    {
        printf("Encryption complete \n");
        if(uncipher_buffer("/Users/vdrouin/mssis/mssis-tp/ssl_tls/ciphered", "/Users/vdrouin/mssis/mssis-tp/ssl_tls/unciphered", "/Users/vdrouin/mssis/mssis-tp/ssl_tls/rsa_pub.pem", "/Users/vdrouin/mssis/mssis-tp/ssl_tls/rsa.pem") == 0)
        {
            printf("Decryption completed \n");
        }
        else
        {
            printf("Could not decrypt file!\n");
            goto exit;
        }
    }
    else
    {
        printf("Could not encrypt file!\n");
        goto exit;
    }
    ret = 0;

exit:
    return ret;
}
