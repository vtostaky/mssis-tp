#ifndef _PROTECT_BUFFER_H
#define _PROTECT_BUFFER_H

#define SALT_SZ 8
#define IV_SZ 16
#define BLOCK_SZ 16
#define HASH_SZ 32
#define MAX_ITER 1000000

/**
 * @param [out] output      ciphered buffer
 * @param [in]  output_len  ciphered buffer length in bytes      
 * @param [in]  input       plain text buffer
 * @param [in]  input_len   plain text buffer length in bytes
 * @param [in]  master_key  master key (km)
 * @param [in]  key_len     master key length in bytes
 * @param [in]  salt        salt
 * @param [in]  salt_len    salt length in bytes
 * @return      0 if OK, 1 else
 */
int protect_buffer(unsigned char **output, int *output_len,
        unsigned char *input, int input_len,
        unsigned char *master_key, int key_len,
        unsigned char *salt, int salt_len);

/**
 * @param [out] output      unciphered buffer
 * @param [in]  output_len  unciphered buffer length in bytes      
 * @param [in]  input       ciphered buffer
 * @param [in]  input_len   ciphered buffer length in bytes
 * @param [in]  master_key  master key (km)
 * @param [in]  key_len     master key length in bytes
 * @param [in]  salt_len    salt length in bytes
 * @return      0 if OK, 1 else
 */
int unprotect_buffer(unsigned char **output, int *output_len,
        unsigned char *input, int input_len,
        unsigned char *master_key, int key_len,
        int salt_len);

/**
 * @param [out] key (32 bytes)
 * @param [in]  passwd      user password
 * @param [in]  salt        salt
 * @param [in]  salt_len    salt length in bytes
 * @param [in]  iterations  number of iterations
 * @return      0 if OK, 1 else
 */
int deriv_passwd(unsigned char *key, char* passwd, unsigned char* salt, int salt_len, unsigned int iterations);

/**
 * @param [out] alea            buffer for the alea to be generated
 * @param [in]  alea_length     length of the expected alea
 * @return      0 if OK, 1 else
 */
int gen_alea(unsigned char *alea, int alea_length);
#endif
