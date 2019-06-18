#ifndef __STM32F0_INTERNAL_H
#define __STM32F0_INTERNAL_H
// Local definitions for STM32F0 code

#include "stm32f0xx.h"

#define GPIO(PORT, NUM) (((PORT)-'A') * 16 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 16)

extern uint8_t const avail_pins[];

void gpio_init(void);
void TimerInit(void);

#endif // internal.h
