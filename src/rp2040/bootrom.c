// Hardware interface to bootrom on rp2040
//
// Copyright (C) 2021  Lasse Dalegaard <dalegaard@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint16_t, uint32_t, uintptr_t
#include <string.h> // memcpy
#include "compiler.h" // noinline, __section

#define ROM_TABLE_CODE(c1, c2) ((c1) | ((c2) << 8))

// All functions in here need to be RAM-resident, as we may need
// to (especially for the flash functions) call while the XIP layer
// is unavailable.
#define noinline_ram noinline __section(".ramfunc.read_chip_id")

static void * noinline_ram
rom_func_lookup(uint32_t code)
{
  // Table and lookup function are provided by the BOOTROM
  void *(*fn)(uint16_t *, uint32_t) =
      (void *)(uintptr_t)(*(uint16_t *)0x18);
  uint16_t *table = (uint16_t *)(uintptr_t)(*(uint16_t *)0x14);
  return fn(table, code);
}

void noinline_ram
reset_to_usb_boot(uint32_t gpio_activity_pin_mask
                  , uint32_t disable_interface_mask)
{
  void (*fn)(uint32_t, uint32_t) = rom_func_lookup(ROM_TABLE_CODE('U', 'B'));
  fn(gpio_activity_pin_mask, disable_interface_mask);
}

void noinline_ram
connect_internal_flash(void)
{
  void (*fn)(void) = rom_func_lookup(ROM_TABLE_CODE('I', 'F'));
  fn();
}

void noinline_ram
flash_exit_xip(void)
{
  void (*fn)(void) = rom_func_lookup(ROM_TABLE_CODE('E', 'X'));
  fn();
}

void noinline_ram
flash_flush_cache(void)
{
  void (*fn)(void) = rom_func_lookup(ROM_TABLE_CODE('F', 'C'));
  fn();
}
