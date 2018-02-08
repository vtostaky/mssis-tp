#ifndef _GEN_KEY_H
#define _GEN_KEY_H

/**
 * @param [out] key         key generated
 * @param [in]  key_length  key length in bytes
 * @return      0 if OK, 1 else
 */

int gen_key(unsigned char *key, int key_length);

#endif
