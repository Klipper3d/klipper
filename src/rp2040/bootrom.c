// Hardware interface to bootrom on rp2040
//
// Copyright (C) 2021  Lasse Dalegaard <dalegaard@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint16_t, uint32_t, uintptr_t

static void *
rom_func_lookup(uint32_t code)
{
  // Table and lookup function are provided by the BOOTROM
  void *(*fn)(uint16_t *, uint32_t) =
      (void *)(uintptr_t)(*(uint16_t *)0x18);
  uint16_t *table = (uint16_t *)(uintptr_t)(*(uint16_t *)0x14);
  return fn(table, code);
}

void
reset_to_usb_boot(uint32_t gpio_activity_pin_mask
                  , uint32_t disable_interface_mask)
{
  void (*reset_to_usb_boot)(uint32_t, uint32_t) = rom_func_lookup(0x4255);
  reset_to_usb_boot(gpio_activity_pin_mask, disable_interface_mask);
}
