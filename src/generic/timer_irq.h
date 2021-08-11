#ifndef __GENERIC_TIMER_IRQ_H
#define __GENERIC_TIMER_IRQ_H

uint32_t timer_dispatch_many(void);
uint8_t timer_is_before(uint32_t time1, uint32_t time2);

#endif // timer_irq.h
