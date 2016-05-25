#ifndef __SIMU_MISC_H
#define __SIMU_MISC_H

#include <stdint.h>

// main.c
char *console_get_input(uint8_t *plen);
void console_pop_input(uint8_t len);
char *console_get_output(uint8_t len);
void console_push_output(uint8_t len);

static inline size_t alloc_maxsize(size_t reqsize) {
    return reqsize;
}

#define HAVE_OPTIMIZED_CRC 0
static inline uint16_t _crc16_ccitt(char *buf, uint8_t len) {
    return 0;
}

#endif // misc.h
