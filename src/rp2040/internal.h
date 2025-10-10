#ifndef __RP2040_INTERNAL_H
#define __RP2040_INTERNAL_H
// Local definitions for RPxxxx chips

#include "autoconf.h" // CONFIG_MACH_RP2040

#if CONFIG_MACH_RP2040
  #include "RP2040.h"
#elif CONFIG_MACH_RP2350
  #include "RP2350.h"
#endif

void enable_pclock(uint32_t reset_bit);
int is_enabled_pclock(uint32_t reset_bit);
uint32_t get_pclock_frequency(uint32_t reset_bit);
void gpio_peripheral(uint32_t gpio, int func, int pull_up);
void bootrom_reboot_usb_bootloader(void);
void bootrom_read_unique_id(uint8_t *out, uint32_t maxlen);

// Force a function to run from ram
#define UNIQSEC __FILE__ "." __stringify(__LINE__)
#define _ramfunc noinline __section(".ramfunc." UNIQSEC)

#endif // internal.h
