#ifndef __SIMU_TIMER_H
#define __SIMU_TIMER_H

#include <stdint.h>

#define TIMER_MIN_TICKS 100

uint32_t timer_from_ms(uint32_t ms);
uint32_t timer_read_time(void);
void timer_set_next(uint32_t next);

#endif // timer.h
