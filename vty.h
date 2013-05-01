#ifndef VTY_H
#define VTY_H

void vty_init();
void vty_putc(char c);
void vty_puts(char * s);
void vty_printf(const char * format, ...);

#endif
