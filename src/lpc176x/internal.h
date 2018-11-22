#ifndef __LPC176X_INTERNAL_H
#define __LPC176X_INTERNAL_H
// Local definitions for lpc176x code

#define PCLK_TIMER0 1
#define PCLK_UART0 3
#define PCLK_ADC 12
void enable_peripheral_clock(uint32_t pclk);
void gpio_peripheral(int bank, int pin, int func, int pullup);

#endif // internal.h
