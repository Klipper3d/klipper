#ifndef __HC32F460_INTERNAL_H
#define __HC32F460_INTERNAL_H


// Local definitions for Huada HC32F460

#include "autoconf.h"
#include "hc32f460.h"

// The HC32F460 library uses a port address and a shifted pin bit
//  eg en_result_t PORT_Toggle(en_port_t enPort, uint16_t u16Pin);
//  see hc32f460_gpio.h


// encode and decode gpio ports and pins
#define GPIO(PORT, NUM) (((PORT)-'A') * 16 + (NUM))
#define GPIO2PORT(GPIO) ((GPIO) / 16)
#define GPIO2BIT(GPIO) (1<<((GPIO) % 16))
#define GPIO2PIN(GPIO) ((GPIO) % 16)

#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

void gpio_peripheral(uint32_t pin, int func, int pull_up);

// from local interrupts.c - helper
void IrqRegistration(en_int_src_t irqSrc, IRQn_Type irqType);

#endif // internal.h
