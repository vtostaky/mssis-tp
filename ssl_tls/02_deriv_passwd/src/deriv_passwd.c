#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deriv_passwd.h"
#include "mbedtls/sha256.h"
#include "mbedtls/havege.h"

int gen_alea(unsigned char *alea, int alea_length)
{
    int ret = 1;
    mbedtls_havege_state hs;

    if((alea == NULL) || (alea_length <= 0))
    {
        goto cleanup;
    }

    mbedtls_havege_init( &hs );
    ret = mbedtls_havege_random( &hs, alea, alea_length );
    mbedtls_havege_free( &hs );

cleanup:
    return ret;
}

void deriv_step(mbedtls_sha256_context ctx, unsigned char *hash, char* passwd, unsigned char* salt, int salt_len, int step)
{
    mbedtls_sha256_starts( &ctx, 0 );
    if(step != 0)
        mbedtls_sha256_update( &ctx, hash, 32);

    mbedtls_sha256_update( &ctx, (unsigned char*) passwd, strlen(passwd));
    mbedtls_sha256_update( &ctx, salt, salt_len);
    mbedtls_sha256_update( &ctx, (unsigned char*) &step, sizeof(int));
    mbedtls_sha256_finish( &ctx, hash);
}

int deriv_passwd(unsigned char *key, char* passwd, unsigned char* salt, int salt_len, unsigned int iterations)
{
    int ret = 1;
    int i = 0;
    unsigned char hash[32];
    mbedtls_sha256_context ctx;

    if((key == NULL) || (passwd == NULL) || (salt == NULL) || (salt_len <= 0) || (iterations < 1))
    {
        goto cleanup;
    }

    mbedtls_sha256_init( &ctx );
    
    deriv_step(ctx, hash, passwd, salt, salt_len, i);
    
    for(i = 1; i < iterations ; i++)
    {
        deriv_step(ctx, hash, passwd, salt, salt_len, i);
    }
    memcpy(key, hash, 32);
    mbedtls_sha256_free( &ctx );
    ret = 0;

cleanup:
    return ret;
}
