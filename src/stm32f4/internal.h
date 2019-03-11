#ifndef __STM32F4_INTERNAL_H
#define __STM32F4_INTERNAL_H
// Local definitions for STM32F1 code

#include "stm32f4xx.h"

#define GPIO(PORT, NUM) (((PORT)-'A') * 16 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 16)

extern GPIO_TypeDef *const digital_regs[];
extern uint32_t const digital_pins[];

#endif // internal.h
