#ifndef CONV_H
#define CONV_H

#define UTODEC_BUFF_LEN_HEX 8
#define UTODEC_BUFF_LEN_DEC 11
#define ITODEC_BUFF_LEN_DEC 12

char * utodec(char * buffer, unsigned n);
char * utohex(char * buffer, unsigned n);
char * itodec(char * buffer, int n);

#endif
