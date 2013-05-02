#ifndef STRING_H
#define STRING_H

void * memcpy(void * dest, const void * src, unsigned n);
void * memmove(void * dst, const void * src, unsigned size);
char * strrev(char * s);
unsigned strlen(const char * s);

#endif
