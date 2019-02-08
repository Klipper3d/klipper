#ifndef __ATSAMD_INTERNAL_H
#define __ATSAMD_INTERNAL_H
// Local definitions for atsamd code

#include <stdint.h> // uint32_t
#include "autoconf.h" // CONFIG_MACH_SAMD21A

#if CONFIG_MACH_SAMD21
#include "samd21.h"
#elif CONFIG_MACH_SAMD51
#include "samd51.h"
#endif

#define GPIO(PORT, NUM) (((PORT)-'A') * 32 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 32)
#define GPIO2BIT(PIN) (1<<((PIN) % 32))

void enable_pclock(uint32_t pclk_id, uint32_t pm_id);
uint32_t get_pclock_frequency(uint32_t pclk_id);
void gpio_peripheral(uint32_t gpio, char ptype, int32_t pull_up);

#endif // internal.h
