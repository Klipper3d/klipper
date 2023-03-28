#include <stdint.h>
#include "asm/spr.h"
#include "sched.h"
#include "internal.h"

uint8_t timer_interrupt_pending(void);
void timer_set(uint32_t value);
uint32_t timer_read_time(void);
void timer_reset(void);
void timer_clear_interrupt(void);
void timer_kick(void);
void timer_init(void);
