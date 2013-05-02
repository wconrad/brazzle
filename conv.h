#ifndef CONV_H
#define CONV_H

#define UTODEC_BUFF_LEN 11
#define UTOHEX_BUFF_LEN 9
#define ITODEC_BUFF_LEN 12

char *utodec(char * buffer, unsigned n);
char *utohex(char * buffer, unsigned n);
char *itodec(char * buffer, int n);
unsigned dectou(const char *buffer, const char **endptr);

#endif
