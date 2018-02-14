#ifndef _CIPHER_MULTI_USERS_H
#define _CIPHER_MULTI_USERS_H

#define SALT_SZ 8
#define IV_SZ 16
#define BLOCK_SZ 16
#define HASH_SZ 32
#define RSA_SZ 256
#define MAX_ITER 1000000
#define MAX_READ_SIZE 8192

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
int cipher_buffer(char *path_input_file, char *path_output_file,
        char *path_privkey_sign, char **paths_pukey_users, int nb_users);

int uncipher_buffer(char *path_input_file, char *path_output_file,
        char *path_pubkey_enc,
        char *path_privkey_sign);

int gen_alea(unsigned char *alea, int alea_length);
#endif
