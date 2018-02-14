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

int cipher_user_key(FILE *output_file, char *path_pubkey_user,
        unsigned char *aes_key,
        mbedtls_sha256_context *sha256_ctx)
{
    int ret = -1;
    
    mbedtls_pk_context pk_ctx;
    mbedtls_havege_state hs;
    mbedtls_rsa_context *rsa_ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
    
    unsigned char hash[HASH_SZ];
    unsigned char wrap_key[RSA_SZ];
    unsigned char *pubBuffer = (unsigned char*)malloc(2048*sizeof(char));

    char c = 0x00;

    if((output_file == NULL)||(pubBuffer == NULL)||(path_pubkey_user == NULL)||
            (aes_key == NULL) || (sha256_ctx == NULL))
        goto cleanup;

    //RSA encrypt
    mbedtls_pk_init(&pk_ctx);

    if( ( ret = mbedtls_pk_parse_public_keyfile( &pk_ctx, path_pubkey_user ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_pk_parse_public_keyfile returned -0x%04x\n", -ret );
        goto cleanup;
    }
    
    ret = mbedtls_pk_write_pubkey_pem( &pk_ctx, pubBuffer, 2048);
    if( ret != 0 )
    {
        printf( " failed\n  ! mbedtls_pk_write_pubkey_pem failed with -0x%0x\n", -ret );
        goto cleanup;
    }

    ret = fwrite (&c, sizeof(char), 1, output_file);
    if(ret != 1)
    {
        ret = -1;
        printf("Error while writing separator 0 to output file\n");
        goto cleanup;
    }
    mbedtls_sha256_update( sha256_ctx, (unsigned char*)&c, 1);

    mbedtls_sha256((const unsigned char *)pubBuffer, 2048, hash, 0);    
    ret = fwrite (hash , sizeof(char), HASH_SZ, output_file);
    if(ret != HASH_SZ)
    {
        ret = -1;
        printf("Error while writing key hash to output file\n");
        goto cleanup;
    }
    mbedtls_sha256_update( sha256_ctx, hash, HASH_SZ);
    
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

    ret = fwrite (wrap_key , sizeof(char), RSA_SZ, output_file);
    if(ret != RSA_SZ)
    {
        ret = -1;
        printf("Error while writing wrap key to output file\n");
        goto cleanup;
    }
    mbedtls_sha256_update( sha256_ctx, wrap_key, RSA_SZ);
    
    mbedtls_pk_free(&pk_ctx);
    mbedtls_havege_free( &hs );
    ret = 0;
cleanup:
    secure_memzero(hash, HASH_SZ);
    secure_memzero(wrap_key, RSA_SZ);
    if(pubBuffer != NULL)
        secure_memzero(pubBuffer, 2048);
    free(pubBuffer);
    return ret;
}

int cipher_buffer(char *path_input_file, char *path_output_file,
        char *path_privkey_sign, char **paths_pubkey_users, int nb_users)
{
    int ret = 1;
    int i;
    
    unsigned char IV[IV_SZ];
    unsigned char aes_key[HASH_SZ];
    unsigned char hash[HASH_SZ];
    unsigned char sig[RSA_SZ];
    
    unsigned char* output = NULL;
    unsigned char* padded_input = NULL;
    int input_len = 0;
    int padded_input_len;
    int end_of_file = 0;
    char c = 0x01;

    FILE *input_file = NULL;
    FILE *output_file = NULL;

    mbedtls_aes_context aes_ctx;
    mbedtls_sha256_context sha256_ctx;
    mbedtls_rsa_context *rsa_ctx;
    mbedtls_pk_context pk_ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
    mbedtls_havege_state hs;

    if((path_output_file == NULL) || (path_input_file == NULL) || (path_privkey_sign == NULL) ||
            (paths_pubkey_users == NULL))
    {
        goto cleanup;
    }

    input_file = fopen ( path_input_file , "rb" );
    if (input_file==NULL)
    {
        printf ("Input file open error\n");
        goto cleanup;
    }
    
    output_file = fopen ( path_output_file , "wb" );
    if (output_file==NULL)
    {
        printf ("Output file open error\n");
        goto cleanup;
    }
    
    padded_input = malloc(MAX_READ_SIZE*sizeof(unsigned char));
    if(padded_input == NULL)
        goto cleanup;
    
    output = malloc(MAX_READ_SIZE*sizeof(unsigned char));
    if(output == NULL)
        goto cleanup;

    
    mbedtls_sha256_init( &sha256_ctx );
    mbedtls_sha256_starts( &sha256_ctx, 0 );

    if(gen_alea(aes_key, HASH_SZ) != 0)
    {   
        printf("Could not generate aes key!\n");
        goto cleanup;
    }
    
    print_hex(aes_key, HASH_SZ, "AES key");

    for(i = 0; i < nb_users; i++)
    {
        if(paths_pubkey_users[i] != NULL)
        {
            ret = cipher_user_key(output_file, paths_pubkey_users[i], aes_key, &sha256_ctx);
            if(ret != 0)
                goto cleanup;
        }
        else
            goto cleanup;
    }
    
    ret = fwrite (&c, sizeof(char), 1, output_file);
    if(ret != 1)
    {
        ret = -1;
        printf("Error while writing separator 1 to output file\n");
        goto cleanup;
    }
    mbedtls_sha256_update( &sha256_ctx, (unsigned char*)&c, 1);
    
    if(gen_alea(IV, IV_SZ) != 0)
    {   
        printf("Could not generate IV!\n");
        goto cleanup;
    }
    
    print_hex(IV, IV_SZ, "IV");
    ret = fwrite (IV , sizeof(char), IV_SZ, output_file);
    if(ret != IV_SZ)
    {
        ret = -1;
        printf("Error while writing IV to output file\n");
        goto cleanup;
    }
    mbedtls_sha256_update( &sha256_ctx, IV, IV_SZ);

    mbedtls_aes_init(&aes_ctx);
    mbedtls_aes_setkey_enc(&aes_ctx, aes_key, 256);

    while(!end_of_file)
    {
        // copy 8kB file part into input buffer:
        input_len = fread (padded_input, sizeof(char), MAX_READ_SIZE, input_file);
        if (input_len < MAX_READ_SIZE)
        {
            padded_input_len = (input_len / BLOCK_SZ + 1) * BLOCK_SZ;
            padded_input[input_len] = 0x80;
            for(i = input_len + 1; i < padded_input_len; i++)
            {
                padded_input[i] = 0;
            }
            end_of_file = 1;
        }
        else
            padded_input_len = MAX_READ_SIZE;
        
        mbedtls_aes_crypt_cbc( &aes_ctx,
                MBEDTLS_AES_ENCRYPT,
                padded_input_len,
                IV,
                padded_input,
                output );
        ret = fwrite (output , sizeof(char), padded_input_len, output_file);
        if(ret != padded_input_len)
        {
            ret = -1;
            printf("Error while writing encrypted buffer to output file\n");
            goto cleanup;
        }
        mbedtls_sha256_update( &sha256_ctx, output, padded_input_len);
    } 
    mbedtls_sha256_finish( &sha256_ctx, hash);
    mbedtls_sha256_free(&sha256_ctx);
    mbedtls_aes_free(&aes_ctx);

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

    ret = fwrite (sig , sizeof(char), RSA_SZ, output_file);
    if(ret != RSA_SZ)
    {
        ret = 1;
        printf("Error while writing signature to output file\n");
        goto cleanup;
    }
    
    mbedtls_havege_free( &hs );
    mbedtls_pk_free(&pk_ctx);
    
    ret = 0;

cleanup:
    secure_memzero(IV, IV_SZ);
    secure_memzero(aes_key, HASH_SZ);
    //secure_memzero(sig, RSA_SZ);
    secure_memzero(hash, HASH_SZ);
    if(input_file != NULL)
        fclose(input_file);
    if(output_file != NULL)
        fclose(output_file);
    if(padded_input != NULL)
        secure_memzero(padded_input, MAX_READ_SIZE);
    free(padded_input);
    if(output != NULL)
        secure_memzero(output, MAX_READ_SIZE);
    free(output);
    return ret;
}

int find_wrap_key_in_buffer(FILE *input_file, char *path_pubkey_user, long int *wrap_pos, long int *iv_pos)
{
    unsigned char hash_from_key[HASH_SZ];
    unsigned char hash_from_buffer[HASH_SZ];
    unsigned char wrap_key[RSA_SZ];
    unsigned char *pubBuffer = (unsigned char*)malloc(2048*sizeof(char));
    char c;
    int ret = -1;
    mbedtls_pk_context pk_ctx;
    
    if((input_file == NULL)||(pubBuffer == NULL)||(path_pubkey_user == NULL)||
            (wrap_pos == NULL)||(iv_pos == NULL))
        goto cleanup;
    
    rewind(input_file);

    //RSA encrypt
    mbedtls_pk_init(&pk_ctx);

    if( ( ret = mbedtls_pk_parse_public_keyfile( &pk_ctx, path_pubkey_user ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_pk_parse_public_keyfile returned -0x%04x\n", -ret );
        goto cleanup;
    }
    
    ret = mbedtls_pk_write_pubkey_pem( &pk_ctx, pubBuffer, 2048);
    if( ret != 0 )
    {
        printf( " failed\n  ! mbedtls_pk_write_pubkey_pem failed with -0x%0x\n", -ret );
        goto cleanup;
    }
    mbedtls_pk_free(&pk_ctx);
    
    mbedtls_sha256((const unsigned char *)pubBuffer, 2048, hash_from_key, 0);    

    while(1)
    {
        ret = fread(&c, sizeof(char), 1, input_file); 
        if(c != 0x00 || ret != 1)
        {
            printf("No matching signature found\n");
            goto cleanup;
        }

        ret = fread(hash_from_buffer, sizeof(char), HASH_SZ, input_file); 
        if (ret != HASH_SZ)
        {
            printf ("Hash read error\n");
            goto cleanup;
        }

        if(memcmp(hash_from_buffer, hash_from_key, HASH_SZ) == 0)
        {
            printf("Found correct key!\n");
            *wrap_pos = ftell(input_file);
            fseek( input_file, RSA_SZ, SEEK_CUR );
            break;
        }
        
        fseek( input_file, RSA_SZ, SEEK_CUR );
    }

    while(1)
    {
        ret = fread(&c, sizeof(char), 1, input_file); 
        if((c != 0x00 && c != 0x01) || ret != 1)
        {
            printf("No matching marker found\n");
            goto cleanup;
        }
        if(c == 0x01)
        {
            *iv_pos = ftell(input_file);
            break;
        }
        fseek( input_file, RSA_SZ + HASH_SZ, SEEK_CUR );
    }
    //Found correct wrap key
    ret = 0;
cleanup:
    secure_memzero(wrap_key, RSA_SZ);
    secure_memzero(hash_from_key, HASH_SZ);
    secure_memzero(hash_from_buffer, HASH_SZ);
    if(pubBuffer != NULL)
        secure_memzero(pubBuffer, 2048);
    free(pubBuffer);
    return ret;
}

int uncipher_buffer(char *path_input_file, char *path_output_file,
        char *path_pubkey_user, char *path_privkey_user,
        char *path_pubkey_sign)
{
    int ret = 1;
    int i;
    
    unsigned char IV[IV_SZ];
    unsigned char aes_key[HASH_SZ];
    unsigned char hash[HASH_SZ];
    unsigned char sig[RSA_SZ];
    unsigned char wrap_key[RSA_SZ];
    size_t olen = 0;

    FILE* input_file = NULL;
    FILE* output_file = NULL;
    int input_len = 0;
    int read_input_len = 0;
    int total_read = 0;
    int expected_output_len = 0;
    int output_len = 0;
    unsigned char* input = NULL;
    unsigned char* output = NULL;
    long int wrap_pos = 0;
    long int iv_pos=0;

    mbedtls_aes_context aes_ctx;
    mbedtls_sha256_context sha256_ctx;
    mbedtls_pk_context pk_ctx;
    mbedtls_rsa_context *rsa_ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
    mbedtls_havege_state hs;

    if((path_output_file == NULL) || (path_input_file == NULL) || (path_pubkey_sign == NULL) ||
            (path_privkey_user == NULL) || (path_pubkey_user == NULL))
    {
        goto cleanup;
    }
    
    input_file = fopen ( path_input_file , "rb" );
    if (input_file==NULL)
    {
        printf ("Input file open error\n");
        goto cleanup;
    }
    
    output_file = fopen ( path_output_file , "wb" );
    if (output_file==NULL)
    {
        printf ("Output file open error\n");
        goto cleanup;
    }

    // Input file size
    fseek (input_file , 0 , SEEK_END);
    input_len = ftell (input_file);

    if(input_len < RSA_SZ + RSA_SZ+HASH_SZ+1 + BLOCK_SZ)
    {
        printf("Invalid input file\n");
        goto cleanup;
    }

    input = malloc(MAX_READ_SIZE*sizeof(unsigned char));
    if(input == NULL)
        goto cleanup;
    
    output = malloc(MAX_READ_SIZE*sizeof(unsigned char));
    if(output == NULL)
        goto cleanup;

    //Set position to signature
    fseek ( input_file , input_len - RSA_SZ , SEEK_SET );
    ret = fread(sig, sizeof(char), RSA_SZ, input_file); 
    if (ret != RSA_SZ)
    {
        printf ("Signature read error\n");
        goto cleanup;
    }

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
    
    rewind (input_file);
    mbedtls_sha256_init( &sha256_ctx );
    mbedtls_sha256_starts( &sha256_ctx, 0 );

    while(total_read < input_len - RSA_SZ)
    {
        // copy 8kB file part into input buffer:
        read_input_len = fread (input, sizeof(char), MAX_READ_SIZE, input_file);
        total_read += read_input_len;
        
        if(total_read > input_len - RSA_SZ)
            read_input_len -= (total_read - (input_len - RSA_SZ));

        mbedtls_sha256_update( &sha256_ctx, input, read_input_len);
    } 
    mbedtls_sha256_finish( &sha256_ctx, hash);
    mbedtls_sha256_free(&sha256_ctx);
    
    if( ( ret = mbedtls_rsa_rsassa_pss_verify( rsa_ctx, mbedtls_havege_random, &hs, MBEDTLS_RSA_PUBLIC, md_type, 32, hash, sig ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_rsa_rsassa_pss_verify returned -0x%0x\n\n", -ret );
        goto cleanup;
    }
    mbedtls_havege_free( &hs );
    mbedtls_pk_free(&pk_ctx);

    ret = find_wrap_key_in_buffer(input_file, path_pubkey_user, &wrap_pos, &iv_pos);
    if(ret != 0)
    {
        printf("Failed to find public key in ciphered file\n");
        goto cleanup;
    }
    
    fseek ( input_file , wrap_pos , SEEK_SET );

    ret = fread(wrap_key, sizeof(char), RSA_SZ, input_file); 
    if (ret != RSA_SZ)
    {
        ret = -1;
        printf ("wrap key read error\n");
        goto cleanup;
    }

    fseek ( input_file , iv_pos , SEEK_SET );
    expected_output_len = input_len - RSA_SZ - iv_pos - IV_SZ;
    
    ret = fread(IV, sizeof(char), IV_SZ, input_file); 
    if (ret != IV_SZ)
    {
        ret = -1;
        printf ("IV read error\n");
        goto cleanup;
    }
    print_hex(IV, IV_SZ, "IV");
    
    mbedtls_pk_init(&pk_ctx);
    
    if( ( ret = mbedtls_pk_parse_keyfile( &pk_ctx, path_privkey_user, NULL ) ) != 0 )
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

    total_read = 0;
    while(total_read < expected_output_len)
    {
        secure_memzero(output, MAX_READ_SIZE);
        // copy 8kB file part into input buffer:
        read_input_len = fread (input, sizeof(char), MAX_READ_SIZE, input_file);
        total_read += read_input_len;
        
        if(total_read > expected_output_len)
            read_input_len -= (total_read - expected_output_len);
        
        mbedtls_aes_crypt_cbc( &aes_ctx,
                MBEDTLS_AES_DECRYPT,
                read_input_len,
                IV,
                input,
                output );

        if(total_read > expected_output_len)
        {
            for(i = read_input_len; i > 0; i--)
            {
                if(output[i-1] == 0x80)
                {
                    output_len = i-1;
                    break;
                }
            }
        }
        else
            output_len = read_input_len;

        ret = fwrite ( output, sizeof(char), output_len, output_file);
        if(ret != output_len)
        {
            ret = -1;
            printf("Error while writing unciphered to output file\n");
            goto cleanup;
        }

    }
    mbedtls_aes_free(&aes_ctx);
    
    ret = 0;
cleanup:
    secure_memzero(IV, IV_SZ);
    secure_memzero(aes_key, HASH_SZ);
    secure_memzero(wrap_key, RSA_SZ);
    secure_memzero(sig, RSA_SZ);
    secure_memzero(hash, HASH_SZ);
    if(input_file != NULL)
        fclose(input_file);
    if(output_file != NULL)
        fclose(output_file);
    if(input != NULL)
        secure_memzero(input, MAX_READ_SIZE);
    free(input);
    if(output != NULL)
        secure_memzero(output, MAX_READ_SIZE);
    free(output);
    return ret;
}
