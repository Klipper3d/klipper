// Reboot into stm32 ROM dfu bootloader
//
// Copyright (C) 2019-2022  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "internal.h" // NVIC_SystemReset
#include "board/irq.h" // irq_disable

// Many stm32 chips have a USB capable "DFU bootloader" in their ROM.
// In order to invoke that bootloader it is necessary to reset the
// chip and jump to a chip specific hardware address.
//
// To reset the chip, the dfu_reboot() code sets a flag in memory (at
// an arbitrary position that is unlikely to be overwritten during a
// chip reset), and resets the chip.  If dfu_reboot_check() sees that
// flag on the next boot it will perform a code jump to the ROM
// address.

// Location of ram address to set internal flag
#if CONFIG_MACH_STM32H7
  #define USB_BOOT_FLAG_ADDR (0x24000000 + 0x8000) // Place flag in "AXI SRAM"
#else
  #define USB_BOOT_FLAG_ADDR (CONFIG_RAM_START + CONFIG_RAM_SIZE - 1024)
#endif

// Signature to set in memory to flag that a dfu reboot is requested
#define USB_BOOT_FLAG 0x55534220424f4f54 // "USB BOOT"

// Flag that bootloader is desired and reboot
void
dfu_reboot(void)
{
    if (!CONFIG_STM32_DFU_ROM_ADDRESS || !CONFIG_HAVE_BOOTLOADER_REQUEST)
        return;
    irq_disable();
    uint64_t *bflag = (void*)USB_BOOT_FLAG_ADDR;
    *bflag = USB_BOOT_FLAG;
#if __CORTEX_M >= 7
    SCB_CleanDCache_by_Addr((void*)bflag, sizeof(*bflag));
#endif
    NVIC_SystemReset();
}

// Check if rebooting into system DFU Bootloader
void
dfu_reboot_check(void)
{
    if (!CONFIG_STM32_DFU_ROM_ADDRESS || !CONFIG_HAVE_BOOTLOADER_REQUEST)
        return;
    if (*(uint64_t*)USB_BOOT_FLAG_ADDR != USB_BOOT_FLAG)
        return;
    *(uint64_t*)USB_BOOT_FLAG_ADDR = 0;
    uint32_t *sysbase = (uint32_t*)CONFIG_STM32_DFU_ROM_ADDRESS;
    asm volatile("mov sp, %0\n bx %1"
                 : : "r"(sysbase[0]), "r"(sysbase[1]));
}
