#ifndef _DERIV_PASSWD_H
#define _DERIV_PASSWD_H

/**
 * @param [out] key (32 bytes)
 * @param [in]  passwd      user password
 * @param [in]  salt        salt
 * @param [in]  salt_len    salt length in bytes
 * @param [in]  iterations  number of iterations
 * @return      0 if OK, 1 else
 */

int deriv_passwd(unsigned char *key, char* passwd, unsigned char* salt, int salt_len, unsigned int iterations);

int gen_alea(unsigned char *alea, int alea_length);
#endif
