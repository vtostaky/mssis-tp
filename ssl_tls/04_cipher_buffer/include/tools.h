#ifndef _TOOLS_H
#define _TOOLS_H

/**
 * @param [in/out]  p       pointer to the buffer to be reset to 0
 * @param [in]      len     length of the buffer
 */
void secure_memzero(void * p, size_t len);
/**
 * @param [in]  p       pointer to the buffer to be printed in hexadecimal values
 * @param [in]  length  length of the buffer
 * @param [in]  name    name of the buffer, to be displayed prior to the content
 */
void print_hex(unsigned char* buffer, int length, const char* name);
/**
 * @param [in]  p       pointer to the ASCII buffer to be printed as strings
 * @param [in]  length  length of the buffer
 * @param [in]  name    name of the buffer, to be displayed prior to the content
 */
void print_char(unsigned char* buffer, int length, const char* name);

#endif
