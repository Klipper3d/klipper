#ifndef __AVR_MISC_H
#define __AVR_MISC_H

#include <stdint.h>
#include <util/crc16.h>

// alloc.c
size_t alloc_maxsize(size_t reqsize);

// console.c
char *console_get_input(uint8_t *plen);
void console_pop_input(uint8_t len);
char *console_get_output(uint8_t len);
void console_push_output(uint8_t len);

// Optimized crc16_ccitt for the avr processor
#define HAVE_OPTIMIZED_CRC 1
static inline uint16_t _crc16_ccitt(char *buf, uint8_t len) {
    uint16_t crc = 0xFFFF;
    while (len--)
        crc = _crc_ccitt_update(crc, *buf++);
    return crc;
}

#endif // misc.h
