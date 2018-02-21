#ifndef _TOOLS_H
#define _TOOLS_H

void secure_memzero(void * p, size_t len);
void print_hex(unsigned char* buffer, int length, const char* name);
void print_char(unsigned char* buffer, int length, const char* name);

#endif
