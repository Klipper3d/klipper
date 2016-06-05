#ifndef __GENERIC_MISC_H
#define __GENERIC_MISC_H

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

char *console_get_input(uint8_t *plen);
void console_pop_input(uint8_t len);
char *console_get_output(uint8_t len);
void console_push_output(uint8_t len);

size_t alloc_maxsize(size_t reqsize);

uint16_t crc16_ccitt(char *buf, uint8_t len);

#endif // misc.h
