#ifndef _CIPHER_MULTI_USERS_H
#define _CIPHER_MULTI_USERS_H

#define SALT_SZ 8
#define IV_SZ 16
#define BLOCK_SZ 16
#define HASH_SZ 32
#define RSA_SZ 256
#define MAX_READ_SIZE 8192

/**
 * @param [in]  path_input_file     path to input file
 * @param [in]  path_output_file    path to output file
 * @param [in]  path_privkey_sign   path to private key for signature
 * @param [in]  paths_pubkey_users  array of path that contains all public keys used to cipher Kc
 * @param [in]  nb_users            number of public keys
 * @return      0 if OK, 1 else
 */
int cipher_file(char *path_input_file, char *path_output_file,
        char *path_privkey_sign, char **paths_pukey_users, int nb_users);

/**
 * @param [in]  path_input_file     path to input file
 * @param [in]  path_output_file    path to output file
 * @param [in]  path_pubkey_user    path to user public key
 * @param [in]  path_privkey_user   path to user private key
 * @param [in]  path_pubkey_sign    path to public key for signature
 * @param [in]  nb_users            number of public keys
 * @return      0 if OK, 1 else
 */
int uncipher_file(char *path_input_file, char *path_output_file,
        char *path_pubkey_user,
        char *path_privkey_user,
        char *path_pubkey_sign);

/**
 * @param [out]  alea            buffer for the alea to be generated
 * @param [in]  alea_length     length of the expected alea
 * @return      0 if OK, 1 else
 */
int gen_alea(unsigned char *alea, int alea_length);
#endif
