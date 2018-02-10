#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"
#include "cipher_multi_users.h"
#include "mbedtls/sha256.h"
#include "mbedtls/aes.h"
#include "mbedtls/pk.h"
#include "mbedtls/rsa.h"
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

    mbedtls_aes_context aes_ctx;
    mbedtls_rsa_context *rsa_ctx;
    mbedtls_pk_context pk_ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
    mbedtls_havege_state hs;

    if((output == NULL) || (output_len == NULL) || (input == NULL) || (input_len < 1) || (path_pubkey_enc == NULL) || (path_privkey_sign == NULL))
    {
        goto cleanup;
    }
 
    if(gen_alea(IV, IV_SZ) != 0)
    {   
        printf("Could not generate IV!\n");
        goto cleanup;
    }

    if(gen_alea(aes_key, HASH_SZ) != 0)
    {   
        printf("Could not generate aes key!\n");
        goto cleanup;
    }

    *output_len = IV_SZ + RSA_SZ + padded_input_len + RSA_SZ;
    *output = malloc((*output_len)*sizeof(unsigned char));

    if(*output == NULL)
        goto cleanup;

    memcpy(*output, IV, IV_SZ);
    
    padded_input = (unsigned char*)malloc(padded_input_len*sizeof(unsigned char));
    if(padded_input == NULL)
        goto cleanup;
    memcpy(padded_input, input, input_len);
    padded_input[input_len] = 0x80;
    for(i = input_len + 1; i < padded_input_len; i++)
    {
        padded_input[i] = 0;
    }
    
    print_hex(aes_key, HASH_SZ, "AES key");
    mbedtls_aes_init(&aes_ctx);
    mbedtls_aes_setkey_enc(&aes_ctx, aes_key, 256);

    mbedtls_aes_crypt_cbc( &aes_ctx,
                    MBEDTLS_AES_ENCRYPT,
                    padded_input_len,
                    IV,
                    padded_input,
                    *output + IV_SZ + RSA_SZ );
    mbedtls_aes_free(&aes_ctx);

    //RSA encrypt
    mbedtls_pk_init(&pk_ctx);

    if( ( ret = mbedtls_pk_parse_public_keyfile( &pk_ctx, path_pubkey_enc ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_pk_parse_public_keyfile returned -0x%04x\n", -ret );
        goto cleanup;
    }
    
    rsa_ctx = mbedtls_pk_rsa(pk_ctx);
    mbedtls_rsa_set_padding( rsa_ctx, MBEDTLS_RSA_PKCS_V21, md_type );
    
    if( ( ret = mbedtls_rsa_check_pubkey( rsa_ctx ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_rsa_check_pubkey failed with -0x%0x\n", -ret );
        goto cleanup;
    }

    mbedtls_havege_init( &hs );
    ret = mbedtls_rsa_rsaes_oaep_encrypt( rsa_ctx, mbedtls_havege_random,
            &hs, MBEDTLS_RSA_PUBLIC, NULL, 0,
            HASH_SZ, aes_key, wrap_key );
    if( ret != 0 )
    {
        printf( " failed\n  ! mbedtls_rsa_aes_oaep_encrypt returned %d\n\n", ret );
        goto cleanup;
    }

    memcpy(*output + IV_SZ, wrap_key, RSA_SZ); 
    mbedtls_havege_free( &hs );
    mbedtls_pk_free(&pk_ctx);

    mbedtls_sha256((const unsigned char *)*output, *output_len - RSA_SZ, hash, 0);    

    mbedtls_pk_init(&pk_ctx);
    
    if( ( ret = mbedtls_pk_parse_keyfile( &pk_ctx, path_privkey_sign, NULL ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_pk_parse_keyfile returned -0x%04x\n", -ret );
        goto cleanup;
    }
    
    rsa_ctx = mbedtls_pk_rsa(pk_ctx);
    mbedtls_rsa_set_padding( rsa_ctx, MBEDTLS_RSA_PKCS_V21, md_type );

    if( ( ret = mbedtls_rsa_check_privkey( rsa_ctx ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_rsa_check_privkey failed with -0x%0x\n", -ret );
        goto cleanup;
    }

    mbedtls_havege_init( &hs );
    if( ( ret = mbedtls_rsa_rsassa_pss_sign( rsa_ctx, mbedtls_havege_random, &hs, MBEDTLS_RSA_PRIVATE, md_type, 32, hash, sig ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_rsa_rsassa_pss_sign returned -0x%0x\n\n", -ret );
        goto cleanup;
    }

    memcpy(*output + *output_len - RSA_SZ, sig, RSA_SZ); 
    
    mbedtls_havege_free( &hs );
    mbedtls_pk_free(&pk_ctx);
    
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
    mbedtls_pk_context pk_ctx;
    mbedtls_rsa_context *rsa_ctx;
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
 
    mbedtls_pk_init(&pk_ctx);

    if( ( ret = mbedtls_pk_parse_public_keyfile( &pk_ctx, path_pubkey_sign ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_pk_parse_public_keyfile returned -0x%04x\n", -ret );
        goto cleanup;
    }
    
    rsa_ctx = mbedtls_pk_rsa(pk_ctx);
    mbedtls_rsa_set_padding( rsa_ctx, MBEDTLS_RSA_PKCS_V21, md_type );
    
    if( ( ret = mbedtls_rsa_check_pubkey( rsa_ctx ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_rsa_check_pubkey failed with -0x%0x\n", -ret );
        goto cleanup;
    }

    mbedtls_havege_init( &hs );
    mbedtls_sha256(input, input_len - RSA_SZ, hash, 0); 
    memcpy(sig, input + input_len - RSA_SZ, RSA_SZ); 
    if( ( ret = mbedtls_rsa_rsassa_pss_verify( rsa_ctx, mbedtls_havege_random, &hs, MBEDTLS_RSA_PUBLIC, md_type, 32, hash, sig ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_rsa_rsassa_pss_verify returned -0x%0x\n\n", -ret );
        goto cleanup;
    }
    mbedtls_havege_free( &hs );
    mbedtls_pk_free(&pk_ctx);

    memcpy(IV, input, IV_SZ);
    
    mbedtls_pk_init(&pk_ctx);
    memcpy(wrap_key, input + IV_SZ, RSA_SZ);
    
    if( ( ret = mbedtls_pk_parse_keyfile( &pk_ctx, path_privkey_dec, NULL ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_pk_parse_keyfile returned -0x%04x\n", -ret );
        goto cleanup;
    }
    
    rsa_ctx = mbedtls_pk_rsa(pk_ctx);
    mbedtls_rsa_set_padding( rsa_ctx, MBEDTLS_RSA_PKCS_V21, md_type );

    if( ( ret = mbedtls_rsa_check_privkey( rsa_ctx ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_rsa_check_privkey failed with -0x%0x\n", -ret );
        goto cleanup;
    }
    
    mbedtls_havege_init( &hs );
    ret = mbedtls_rsa_rsaes_oaep_decrypt( rsa_ctx, mbedtls_havege_random,
            &hs, MBEDTLS_RSA_PRIVATE, NULL, 0,
            &olen, wrap_key, aes_key, HASH_SZ );
    if( ret != 0 )
    {
        printf( " failed\n  ! mbedtls_rsa_aes_oaep_decrypt returned -0x%0x\n\n", -ret );
        goto cleanup;
    }

    mbedtls_havege_free( &hs );
    mbedtls_pk_free(&pk_ctx);

    print_hex(aes_key, HASH_SZ, "AES key");
    mbedtls_aes_init(&aes_ctx);
    mbedtls_aes_setkey_dec(&aes_ctx, aes_key, RSA_SZ);

    mbedtls_aes_crypt_cbc( &aes_ctx,
                    MBEDTLS_AES_DECRYPT,
                    *output_len,
                    IV,
                    input + IV_SZ + RSA_SZ,
                    *output );
    mbedtls_aes_free(&aes_ctx);
    
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
