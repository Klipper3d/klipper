#ifndef __STM32F4_INTERNAL_H
#define __STM32F4_INTERNAL_H
// Local definitions for STM32F4 code

#include "stm32f4xx.h"

extern GPIO_TypeDef * const digital_regs[];

#define GPIO(PORT, NUM) (((PORT)-'A') * 16 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 16)
#define GPIO2BIT(PIN) (1<<((PIN) % 16))

#define GPIO_INPUT 0
#define GPIO_OUTPUT 1
#define GPIO_FUNCTION(fn) (2 | ((fn) << 4))
#define GPIO_ANALOG 3

void enable_pclock(uint32_t periph_base);
int is_enabled_pclock(uint32_t periph_base);
uint32_t get_pclock_frequency(uint32_t periph_base);
void clock_setup(void);
void gpio_peripheral(uint32_t gpio, uint32_t mode, int pullup);

#endif // internal.h
