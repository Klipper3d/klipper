#ifndef __GENERIC_MISC_H
#define __GENERIC_MISC_H

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

char *console_get_input(uint8_t *plen);
void console_pop_input(uint8_t len);
char *console_get_output(uint8_t len);
void console_push_output(uint8_t len);

uint32_t timer_from_us(uint32_t us);
uint8_t timer_is_before(uint32_t time1, uint32_t time2);
uint32_t timer_read_time(void);
void timer_periodic(void);

void *alloc_chunk(size_t size);
void *alloc_chunks(size_t size, size_t count, size_t *avail);

uint16_t crc16_ccitt(char *buf, uint8_t len);

#endif // misc.h
