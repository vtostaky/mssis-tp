#ifndef _CIPHER_BUFFER_H
#define _CIPHER_BUFFER_H

#define SALT_SZ 8
#define IV_SZ 16
#define BLOCK_SZ 16
#define HASH_SZ 32
#define RSA_SZ 256
#define MAX_ITER 1000000

/**
 * @param [out] output              ciphered buffer
 * @param [in]  output_len          ciphered buffer length in bytes 
 * @param [in]  input               plain text buffer
 * @param [in]  input_len           plain text buffer length in bytes
 * @param [in]  path_pubkey_enc     path to RSA public key used for encryption
 * @param [in]  path_privkey_sign   path to RSA private key used for signature
 * @return      0 if OK, 1 else
 */
int cipher_buffer(unsigned char **output, int *output_len,
        unsigned char *input, int input_len,
        char *path_pubkey_enc,
        char *path_privkey_sign);

/**
 * @param [out] output              unciphered buffer
 * @param [in]  output_len          unciphered buffer length in bytes 
 * @param [in]  input               ciphered buffer
 * @param [in]  input_len           ciphered buffer length in bytes
 * @param [in]  path_pubkey_sign    path to RSA public key used for integrity check
 * @param [in]  path_privkey_dec    path to RSA private key used for decryption
 * @return      0 if OK, 1 else
 */
int uncipher_buffer(unsigned char **output, int *output_len,
        unsigned char *input, int input_len,
        char *path_pubkey_sign,
        char *path_privkey_dec);

/**
 * @param [out] alea            buffer for the alea to be generated
 * @param [in]  alea_length     length of the expected alea
 * @return      0 if OK, 1 else
 */
int gen_alea(unsigned char *alea, int alea_length);
#endif
