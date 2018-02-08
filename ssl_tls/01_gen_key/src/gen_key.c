#include <stdio.h>
#include <stdlib.h>
#include "gen_key.h"
#include "mbedtls/havege.h"

/**
 * @param [out] key         key generated
 * @param [in]  key_length  key length in bytes
 * @return      0 if OK, 1 else
 */

int gen_key(unsigned char *key, int key_length)
{
    int ret = 1;
    mbedtls_havege_state hs;

    if((key == NULL) || (key_length <= 0))
    {
        goto cleanup;
    }

    mbedtls_havege_init( &hs );
    ret = mbedtls_havege_random( &hs, key, key_length );
    mbedtls_havege_free( &hs );

cleanup:
    return ret;
}

int main(int argc, char **argv)
{
    int length, i;
    int ret = -1;
    unsigned char *output_key;
   
    if(argc > 1)
        length = atoi(argv[1]);
    else
    {
        printf("Usage : %s <key_length>\n",argv[0]);
        goto exit;
    }

    output_key = malloc(length*sizeof(char));

    if(output_key != NULL)
    {
        if(gen_key(output_key, length) == 0)
        {
            printf("key (%d) = ", length);
            for(i = 0; i < length; i++)
                printf("%02x",output_key[i]);
            printf("\n");
        }
        else
        {
            printf("Could not generate key!\n");
            goto exit;
        }
        free(output_key);
        ret = 0;
    }
exit:
    return ret;
}
