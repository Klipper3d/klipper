#ifndef __SIMU_TIMER_H
#define __SIMU_TIMER_H

#include <stdint.h>

uint32_t timer_from_ms(uint32_t ms);
void timer_periodic(void);
uint32_t timer_read_time(void);
uint8_t timer_set_next(uint32_t next);
uint8_t timer_try_set_next(uint32_t next);

#endif // timer.h
