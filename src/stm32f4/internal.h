#ifndef __STM32_INTERNAL_H
#define __STM32_INTERNAL_H

#include "stm32.h"

#define GPIO(PORT, NUM) (((PORT)-'A') * 16 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 16)

extern GPIO_TypeDef *const digital_regs[];
extern uint32_t const digital_pins[];

void udelay(uint32_t usecs);

#endif // internal.h
