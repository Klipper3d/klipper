// Main starting point for AVR boards.
//
// Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <avr/io.h>      // AVR_STACK_POINTER_REG
#include <util/crc16.h>  // _crc_ccitt_update

#include "autoconf.h"    // CONFIG_MCU
#include "board/misc.h"  // dynmem_start
#include "command.h"     // DECL_CONSTANT
#include "irq.h"         // irq_enable
#include "pindefine.h"
#include "sched.h"  // sched_main

#if CONFIG_SERIAL_BAUD > 115200
#warning "Lgt8f328p always using cheap usb to uart ic, " \
         "some of lgt8f328p cannot support high baudrate"
#endif

DECL_CONSTANT_STR("MCU", CONFIG_MCU);

/****************************************************************
 * Dynamic memory
 ****************************************************************/

// Return the start of memory available for dynamic allocations
void *dynmem_start(void) {
  extern char _end;
  return &_end;
}

// Return the end of memory available for dynamic allocations
void *dynmem_end(void) {
  return (void *)ALIGN(AVR_STACK_POINTER_REG, 256) - CONFIG_LGT_STACK_SIZE;
}

// Optimized crc16_ccitt for the avr processor
uint16_t crc16_ccitt(uint8_t *buf, uint_fast8_t len) {
  uint16_t crc = 0xFFFF;
  while (len--) crc = _crc_ccitt_update(crc, *buf++);
  return crc;
}

void set_cpu_speed(void) {
// fix baudrate
#if CONFIG_SERIAL_BAUD == 115200 && CONFIG_CLOCK_FREQ == 16000000L
  const uint8_t CAL_V = 6;
  if ((uint8_t)RCMCAL >= CAL_V) {
    RCMCAL -= CAL_V;
  } else {
    RCMCAL = 0;
  }
#endif

  CLKPR = 0x80;
  CLKPR = 0x01;
  // switch to internal crystal
  GPIOR0 = PMCR & 0x9f;
  PMCR = 0x80;
  PMCR = GPIOR0;

  // disable external crystal
  GPIOR0 = PMCR & 0xfb;
  PMCR = 0x80;
  PMCR = GPIOR0;

  CLKPR = 0x80;
// Set the CPU speed
#if CONFIG_CLOCK_FREQ == 32000000L
  CLKPR = 0x00;
#elif CONFIG_CLOCK_FREQ == 16000000L
  CLKPR = 0x01;
#elif CONFIG_CLOCK_FREQ == 8000000L
  CLKPR = 0x02;
#elif CONFIG_CLOCK_FREQ == 4000000L
  CLKPR = 0x03;
#elif CONFIG_CLOCK_FREQ == 2000000L
  CLKPR = 0x04;
#elif CONFIG_CLOCK_FREQ == 1000000L
  CLKPR = 0x05;
#endif
}

// Main entry point for avr code.
int main(void) {
  set_cpu_speed();
  irq_enable();
  sched_main();
  return 0;
}
