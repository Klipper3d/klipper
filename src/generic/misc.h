#ifndef __GENERIC_MISC_H
#define __GENERIC_MISC_H

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

char *console_get_input(uint8_t *plen);
void console_pop_input(uint8_t len);
char *console_get_output(uint8_t len);
void console_push_output(uint8_t len);

uint32_t timer_from_us(uint32_t us);
void timer_periodic(void);
uint32_t timer_read_time(void);
uint8_t timer_set_next(uint32_t next);
uint8_t timer_try_set_next(uint32_t next);

size_t alloc_maxsize(size_t reqsize);

uint16_t crc16_ccitt(char *buf, uint8_t len);

#endif // misc.h
