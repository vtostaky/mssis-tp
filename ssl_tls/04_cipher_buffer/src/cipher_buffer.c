#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"
#include "cipher_buffer.h"
#include "mbedtls/sha256.h"
#include "mbedtls/aes.h"
#include "mbedtls/pk.h"
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

int cipher_buffer(unsigned char **output, int *output_len,
        unsigned char *input, int input_len,
        char *path_pubkey_enc,
        char *path_privkey_sign)
{
    int ret = 1;
    int i;
    int padded_input_len =(input_len/BLOCK_SZ + 1)*BLOCK_SZ;
    
    unsigned char IV[IV_SZ];
    unsigned char aes_key[HASH_SZ];
    unsigned char* padded_input = NULL;
    unsigned char hash[HASH_SZ];
    unsigned char sig[RSA_SZ];
    unsigned char wrap_key[RSA_SZ];
    size_t siglen;
    size_t olen = 0;

    mbedtls_aes_context aes_ctx;
    mbedtls_pk_context rsa_ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
    mbedtls_havege_state hs;

    if((output == NULL) || (output_len == NULL) || (input == NULL) || (input_len < 1) || (path_pubkey_enc == NULL) || (path_privkey_sign == NULL))
    {
        goto cleanup;
    }
 
    //Generate input vector
    if(gen_alea(IV, IV_SZ) != 0)
    {   
        printf("Could not generate IV!\n");
        goto cleanup;
    }

    //Generate AES key
    if(gen_alea(aes_key, HASH_SZ) != 0)
    {   
        printf("Could not generate aes key!\n");
        goto cleanup;
    }

    *output_len = IV_SZ + RSA_SZ + padded_input_len + RSA_SZ;
    *output = malloc((*output_len)*sizeof(unsigned char));

    if(*output == NULL)
        goto cleanup;
    secure_memzero(*output, *output_len);

    //Copy generated IV into output buffer
    memcpy(*output, IV, IV_SZ);

    //Prepare input for encryption    
    padded_input = (unsigned char*)malloc(padded_input_len*sizeof(unsigned char));
    if(padded_input == NULL)
        goto cleanup;
    secure_memzero(padded_input, padded_input_len);
    
    memcpy(padded_input, input, input_len);
    padded_input[input_len] = 0x80;
    for(i = input_len + 1; i < padded_input_len; i++)
    {
        padded_input[i] = 0;
    }
    
    print_hex(aes_key, HASH_SZ, "AES key");
    //AES encryption of input buffer into output buffer
    mbedtls_aes_init(&aes_ctx);
    mbedtls_aes_setkey_enc(&aes_ctx, aes_key, RSA_SZ);

    mbedtls_aes_crypt_cbc( &aes_ctx,
                    MBEDTLS_AES_ENCRYPT,
                    padded_input_len,
                    IV,
                    padded_input,
                    *output + IV_SZ + RSA_SZ );
    mbedtls_aes_free(&aes_ctx);

    //RSA encryption of the AES public key
    mbedtls_pk_init(&rsa_ctx);

    if( ( ret = mbedtls_pk_parse_public_keyfile( &rsa_ctx, path_pubkey_enc ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_pk_parse_public_keyfile returned -0x%04x\n", -ret );
        goto cleanup;
    }

    mbedtls_havege_init( &hs );
    if( ( ret = mbedtls_pk_encrypt( &rsa_ctx, aes_key, HASH_SZ,
                    wrap_key, &olen, sizeof(wrap_key),
                    mbedtls_havege_random, &hs ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_pk_encrypt returned -0x%04x\n", -ret );
        goto cleanup;
    }

    //Copy encrypted AES key (wrap key) into output buffer, before AES encrypted content
    memcpy(*output + IV_SZ, wrap_key, olen); 
    mbedtls_havege_free( &hs );
    mbedtls_pk_free(&rsa_ctx);

    //Compute hash of the output buffer
    mbedtls_sha256((const unsigned char *)*output, *output_len - RSA_SZ, hash, 0);    
    mbedtls_pk_init(&rsa_ctx);
    
    //Sign computed hash
    if( ( ret = mbedtls_pk_parse_keyfile( &rsa_ctx, path_privkey_sign, NULL ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_pk_parse_keyfile returned -0x%04x\n", -ret );
        goto cleanup;
    }

    if( ( ret = mbedtls_pk_sign( &rsa_ctx, md_type,
             hash, 0,
             sig, &siglen,
             NULL, NULL )) != 0)
    {
        printf( " failed\n  ! mbedtls_pk_sign returned -0x%04x\n", -ret );
        goto cleanup;
    }

    //Place signature at the end of output buffer
    memcpy(*output + *output_len - RSA_SZ, sig, siglen); 
    
    mbedtls_pk_free(&rsa_ctx);
    
    ret = 0;

cleanup:
    secure_memzero(IV, IV_SZ);
    secure_memzero(aes_key, HASH_SZ);
    secure_memzero(wrap_key, RSA_SZ);
    //secure_memzero(sig, RSA_SZ);
    secure_memzero(hash, HASH_SZ);
    if(padded_input != NULL)
        secure_memzero(padded_input, padded_input_len);
    free(padded_input);
    return ret;
}

int uncipher_buffer(unsigned char **output, int *output_len,
        unsigned char *input, int input_len,
        char *path_pubkey_sign,
        char *path_privkey_dec)
{
    int ret = 1;
    int i;
    
    unsigned char IV[IV_SZ];
    unsigned char aes_key[HASH_SZ];
    unsigned char hash[HASH_SZ];
    unsigned char sig[RSA_SZ];
    unsigned char wrap_key[RSA_SZ];
    size_t olen = 0;

    mbedtls_aes_context aes_ctx;
    mbedtls_pk_context rsa_ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
    mbedtls_havege_state hs;

    if((output == NULL) || (output_len == NULL) || (input == NULL) || (input_len < 1) || (path_pubkey_sign == NULL) || (path_privkey_dec == NULL))
    {
        goto cleanup;
    }
    
    *output_len = input_len - (IV_SZ + RSA_SZ + RSA_SZ);
    *output = malloc((*output_len)*sizeof(unsigned char));
    if(*output == NULL)
        goto cleanup;
    secure_memzero(*output, *output_len);
 
    mbedtls_pk_init(&rsa_ctx);

    if( ( ret = mbedtls_pk_parse_public_keyfile( &rsa_ctx, path_pubkey_sign ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_pk_parse_public_keyfile returned -0x%04x\n", -ret );
        goto cleanup;
    }
    
    //Extract signature from buffer and compare it to computed one
    memcpy(sig, input + input_len - RSA_SZ, RSA_SZ);  
    mbedtls_sha256(input, input_len - RSA_SZ, hash, 0);    
    if( ( ret = mbedtls_pk_verify( &rsa_ctx, md_type,
             hash, 0,
             sig, RSA_SZ)) != 0)
    {
        printf( " failed\n  ! mbedtls_pk_verify returned -0x%04x\n", -ret );
        goto cleanup;
    }
    mbedtls_pk_free(&rsa_ctx);

    //Extract IV from input buffer
    memcpy(IV, input, IV_SZ);
    
    mbedtls_pk_init(&rsa_ctx);
    //Extract wrap key from input buffer
    memcpy(wrap_key, input + IV_SZ, RSA_SZ);
    //RSA decrypt wrap key into AES key
    if( ( ret = mbedtls_pk_parse_keyfile( &rsa_ctx, path_privkey_dec, NULL ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_pk_parse_keyfile returned -0x%04x\n", -ret );
        goto cleanup;
    }
    
    mbedtls_havege_init( &hs );
    if( ( ret = mbedtls_pk_decrypt( &rsa_ctx, wrap_key, sizeof(wrap_key),
                    aes_key, &olen, HASH_SZ,
                    mbedtls_havege_random, &hs ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_pk_decrypt returned -0x%04x\n", -ret );
        goto cleanup;
    }

    mbedtls_havege_free( &hs );
    mbedtls_pk_free(&rsa_ctx);

    print_hex(aes_key, HASH_SZ, "AES key");
    //AES decrypt input buffer into output buffer
    mbedtls_aes_init(&aes_ctx);
    mbedtls_aes_setkey_dec(&aes_ctx, aes_key, RSA_SZ);

    mbedtls_aes_crypt_cbc( &aes_ctx,
                    MBEDTLS_AES_DECRYPT,
                    *output_len,
                    IV,
                    input + IV_SZ + RSA_SZ,
                    *output );
    mbedtls_aes_free(&aes_ctx);
    
    //Remove padding
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
    secure_memzero(IV, IV_SZ);
    secure_memzero(aes_key, HASH_SZ);
    secure_memzero(wrap_key, RSA_SZ);
    secure_memzero(sig, RSA_SZ);
    secure_memzero(hash, HASH_SZ);
    return ret;
}
