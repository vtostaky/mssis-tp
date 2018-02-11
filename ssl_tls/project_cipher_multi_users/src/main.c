#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cipher_multi_users.h"
#include "tools.h"

int main(int argc, char **argv)
{
    int ret = -1;

    if(cipher_buffer("/Users/vdrouin/mssis/mssis-tp/ssl_tls/cb.mp3", "/Users/vdrouin/mssis/mssis-tp/ssl_tls/ciphered", "/Users/vdrouin/mssis/mssis-tp/ssl_tls/rsa_pub.pem", "/Users/vdrouin/mssis/mssis-tp/ssl_tls/rsa.pem") == 0)
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
