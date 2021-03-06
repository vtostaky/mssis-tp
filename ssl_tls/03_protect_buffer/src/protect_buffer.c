#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"
#include "protect_buffer.h"
#include "mbedtls/sha256.h"
#include "mbedtls/aes.h"
#include "mbedtls/md.h"
#include "mbedtls/havege.h"


/**
 * Step of password derivation
 * @param [in/out]  ctx         sha256 context used for derivation
 * @param [out]     hash        the resulting hash
 * @param [in]      passwd      user password
 * @param [in]      salt        salt
 * @param [in]      salt_len    salt length in bytes
 * @param [in]      step        the iteration
 */
static void deriv_step(mbedtls_sha256_context *ctx, unsigned char *hash, char* passwd, unsigned char* salt, int salt_len, int step);
/**
 * Master key derivation
 * @param [in/out]  sha256_ctx  sha256 context used for derivation
 * @param [in]      master_key  master key to be derivated
 * @param [in]      key_len     key length in bytes
 * @param [in]      step        the step id
 * @param [out]     res         the resulting derivation
 */
static void deriv_masterkey(mbedtls_sha256_context *sha256_ctx, unsigned char *master_key, int key_len, int step, unsigned char* res);

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

void deriv_masterkey(mbedtls_sha256_context *sha256_ctx, unsigned char *master_key, int key_len, int step, unsigned char* res)
{
    mbedtls_sha256_starts( sha256_ctx, 0 );
    mbedtls_sha256_update( sha256_ctx, master_key, key_len);
    mbedtls_sha256_update( sha256_ctx, (unsigned char*) &step, sizeof(int));
    mbedtls_sha256_finish( sha256_ctx, res);
}

void deriv_step(mbedtls_sha256_context *ctx, unsigned char *hash, char* passwd, unsigned char* salt, int salt_len, int step)
{
    mbedtls_sha256_starts( ctx, 0 );
    if(step != 0)
        mbedtls_sha256_update( ctx, hash, HASH_SZ);

    mbedtls_sha256_update( ctx, (unsigned char*) passwd, strlen(passwd));
    mbedtls_sha256_update( ctx, salt, salt_len);
    mbedtls_sha256_update( ctx, (unsigned char*) &step, sizeof(int));
    mbedtls_sha256_finish( ctx, hash);
}

int deriv_passwd(unsigned char *key, char* passwd, unsigned char* salt, int salt_len, unsigned int iterations)
{
    int ret = 1;
    int i = 0;
    unsigned char hash[HASH_SZ];
    mbedtls_sha256_context ctx;

    if((key == NULL) || (passwd == NULL) || (salt == NULL) || (salt_len <= 0) || (iterations < 1))
    {
        goto cleanup;
    }

    mbedtls_sha256_init( &ctx );
    
    deriv_step(&ctx, hash, passwd, salt, salt_len, i);
    
    for(i = 1; i < iterations ; i++)
    {
        deriv_step(&ctx, hash, passwd, salt, salt_len, i);
    }
    memcpy(key, hash, HASH_SZ);
    mbedtls_sha256_free( &ctx );
    ret = 0;

cleanup:
    return ret;
}

int protect_buffer(unsigned char **output, int *output_len,
        unsigned char *input, int input_len,
        unsigned char *master_key, int key_len,
        unsigned char *salt, int salt_len)
{
    int ret = 1;
    int i;
    int padded_input_len =(input_len/BLOCK_SZ + 1)*BLOCK_SZ;
    
    unsigned char IV[IV_SZ];
    unsigned char aes_key[HASH_SZ];
    unsigned char hash_key[HASH_SZ];
    unsigned char* padded_input = NULL;
    unsigned char hmac_hash[HASH_SZ];

    mbedtls_sha256_context sha256_ctx;
    mbedtls_aes_context aes_ctx;
    mbedtls_md_context_t hmac_ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

    if((output == NULL) || (output_len == NULL) || (input == NULL) || ( master_key == NULL) || (salt == NULL) || (salt_len <1) || (key_len < 1) || (input_len < 1))
    {
        goto cleanup;
    }
    
    if(gen_alea(IV, IV_SZ) != 0)
    {   
        printf("Could not generate IV!\n");
        goto cleanup;
    }

    *output_len = salt_len + IV_SZ + (input_len/BLOCK_SZ + 1)*BLOCK_SZ + HASH_SZ;

    *output = (unsigned char*)malloc(*output_len * sizeof(unsigned char));
    if(*output == NULL)
        goto cleanup;
    secure_memzero(*output, *output_len);
   
    //Prepare input buffer for cipher operation
    padded_input = (unsigned char*)malloc(padded_input_len * sizeof(unsigned char));
    if(padded_input == NULL)
        goto cleanup;
    secure_memzero(padded_input, padded_input_len);

    memcpy(padded_input, input, input_len);

    padded_input[input_len] = 0x80;
    for(i = input_len + 1; i < padded_input_len; i++)
    {
        padded_input[i] = 0;
    }

    //Copy generated salt and IV to output buffer
    memcpy(*output, salt, salt_len);
    memcpy(*output + salt_len, IV, IV_SZ);
   
    //Derivate master key into AES key
    mbedtls_sha256_init( &sha256_ctx );
    deriv_masterkey(&sha256_ctx, master_key, key_len, 0, aes_key);
    mbedtls_sha256_free( &sha256_ctx );
    
    print_hex(aes_key, HASH_SZ, "AES key");
    
    //Encrypt input buffer into output buffer
    mbedtls_aes_init(&aes_ctx);
    mbedtls_aes_setkey_enc(&aes_ctx, aes_key, 256);

    mbedtls_aes_crypt_cbc( &aes_ctx,
                    MBEDTLS_AES_ENCRYPT,
                    padded_input_len,
                    IV,
                    padded_input,
                    *output + salt_len + IV_SZ );
    mbedtls_aes_free(&aes_ctx);

    //Derivate master key into hash
    mbedtls_sha256_init( &sha256_ctx );
    deriv_masterkey(&sha256_ctx, master_key, key_len, 1, hash_key);
    mbedtls_sha256_free( &sha256_ctx );

    mbedtls_md_init( &hmac_ctx );
    //Compute hmac from hash
    mbedtls_md_setup(&hmac_ctx, mbedtls_md_info_from_type(md_type) , 1); //use hmac
    mbedtls_md_hmac_starts(&hmac_ctx, hash_key, HASH_SZ);
    mbedtls_md_hmac_update(&hmac_ctx, (const unsigned char *) *output, *output_len - HASH_SZ);    
    mbedtls_md_hmac_finish(&hmac_ctx, hmac_hash);

    print_hex(hmac_hash, HASH_SZ, "hmac");
    //Put hmac to the end of output buffer
    memcpy(*output + *output_len - HASH_SZ, hmac_hash, HASH_SZ);
    
    mbedtls_md_free(&hmac_ctx);

    ret = 0;
cleanup:
    if(padded_input != NULL)
        secure_memzero(padded_input, padded_input_len);
    free(padded_input);
    return ret;
}

int unprotect_buffer(unsigned char **output, int *output_len,
        unsigned char *input, int input_len, unsigned char* master_key, int key_len, int salt_len)
{
    int ret = 1;
    int i;

    unsigned char IV[IV_SZ];
    unsigned char aes_key[HASH_SZ];
    unsigned char hash_key[HASH_SZ];
    unsigned char hmac_hash[HASH_SZ];
    unsigned char salt[salt_len];

    mbedtls_aes_context aes_ctx;
    mbedtls_sha256_context sha256_ctx;
    mbedtls_md_context_t hmac_ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

    if((output == NULL) || (output_len == NULL) || (master_key == NULL) || (input == NULL) || (input_len < salt_len + IV_SZ + HASH_SZ) || (key_len < 1) || (salt_len < 1))
    {
        goto cleanup;
    }
    *output_len = input_len - (salt_len + IV_SZ + HASH_SZ);

    *output = (unsigned char*)malloc(*output_len * sizeof(unsigned char));
    if(*output == NULL)
        goto cleanup;
    secure_memzero(*output, *output_len);
 
    //Extract salt and IV from input buffer
    memcpy(salt, input, salt_len);
    memcpy(IV, input + salt_len, IV_SZ);

    //Derivate hash from master key
    mbedtls_sha256_init( &sha256_ctx );
    deriv_masterkey(&sha256_ctx, master_key, key_len, 1, hash_key);
    mbedtls_sha256_free( &sha256_ctx );

    mbedtls_md_init( &hmac_ctx );
    //Compute hmac from hash
    mbedtls_md_setup(&hmac_ctx, mbedtls_md_info_from_type(md_type) , 1); //use hmac
    mbedtls_md_hmac_starts(&hmac_ctx, hash_key, HASH_SZ);
    mbedtls_md_hmac_update(&hmac_ctx, (const unsigned char *) input, input_len - HASH_SZ);    
    mbedtls_md_hmac_finish(&hmac_ctx, hmac_hash);

    //Compare computed hmac to hmac in input buffer
    if(memcmp(input + input_len - HASH_SZ, hmac_hash, HASH_SZ) != 0)
    {
        printf("Hash integrity failure!!\n");
        goto cleanup;
    }
    
    mbedtls_md_free(&hmac_ctx);
    //Derivate AES key from master key
    mbedtls_sha256_init( &sha256_ctx );
    deriv_masterkey(&sha256_ctx, master_key, key_len, 0, aes_key);
    mbedtls_sha256_free( &sha256_ctx );
   
    //Decrypt input buffer into output buffer
    mbedtls_aes_init(&aes_ctx);
    mbedtls_aes_setkey_dec(&aes_ctx, aes_key, 256);

    print_hex(aes_key, HASH_SZ, "AES KEY");
    
    mbedtls_aes_crypt_cbc( &aes_ctx,
                    MBEDTLS_AES_DECRYPT,
                    input_len - salt_len - IV_SZ - HASH_SZ,
                    IV,
                    input + salt_len + IV_SZ,
                    *output);
    mbedtls_aes_free(&aes_ctx);

    //Remove padding from output buffer
    for(i = *output_len; i > 0; i--)
    {
        if((*output)[i-1] == 0x80)
        {
            (*output)[i-1] = 0;
            *output_len = i;
            break;
        }
    }

    ret = 0;
cleanup:
    return ret;
}
