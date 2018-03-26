#ifndef _ERROR_H_
#define _ERROR_H_

/* types d'erreur */
#define ERR_MEM   -1
#define ERR_FOPEN -2
#define ERR_FREAD -3
#define ERR_ARG   -4
#define ERR_TREAT -5

void print_err (char *function, char *var, int ERR);

#endif /* _ERROR_H_ */
