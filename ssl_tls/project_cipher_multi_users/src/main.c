#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cipher_multi_users.h"
#include "tools.h"

int main(int argc, char **argv)
{
    int ret = -1;
    int i;

    char *input;
    char *output;
    char *rsa_pub;
    char *rsa_priv;
    char **rsa_encrypt_pub = NULL;
    char *rsa_pub_sender;

    if(argc < 6 ||
            (strncmp(argv[1],"-d", strlen(argv[1])) != 0 && strncmp(argv[1], "-e", strlen(argv[1])) != 0) ||
            (argc != 7 && strncmp(argv[1], "-d", strlen(argv[1])) == 0)
      )
    {
        printf("Usage :\n./multi_protect -e <input_file> <output_file> <my_sign_priv.pem> <my_ciph_pub.pem> [user1_ciph_pub.pem ... [userN_ciph_pub.pem]]\n");
        printf("./multi_protect -d <input_file> <output_file> <my_priv_ciph.pem> <my_pub_ciph.pem> <sender_sign_pub.pem>\n");
        goto exit;
    }
    else
    {
        input = argv[2];
        output = argv[3];
        rsa_priv = argv[4];
        
        if(strncmp(argv[1], "-e", strlen(argv[1])) == 0)
        {
            rsa_encrypt_pub = (char**)malloc((argc-5)*sizeof(char*));
            if(rsa_encrypt_pub == NULL)
                goto exit;
            
            for(i = 5; i < argc; i++)
                rsa_encrypt_pub[i-5] = NULL;
            for(i = 5; i < argc; i++)
            {
                rsa_encrypt_pub[i-5] = strdup(argv[i]);
                if(rsa_encrypt_pub[i-5] == NULL)
                    goto exit;
            }
            if(cipher_file(input, output, rsa_priv, rsa_encrypt_pub, argc-5) == 0)
            {
                printf("Encryption complete \n");
            }
            else
            {
                printf("Could not encrypt file!\n");
                goto exit;
            }
        }
        else
        {
            rsa_pub = argv[5];
            rsa_pub_sender = argv[6];
            if(uncipher_file(input, output, rsa_pub, rsa_priv, rsa_pub_sender) == 0)
            {
                printf("Decryption completed \n");
            }
            else
            {
                printf("Could not decrypt file!\n");
                goto exit;
            }
        }
    }
    ret = 0;

exit:
    if(rsa_encrypt_pub != NULL)
    {
        for(i = 0; i < argc-5; i++)
        {
            free(rsa_encrypt_pub[i]);
        }
    }
    free(rsa_encrypt_pub);
    return ret;
}
