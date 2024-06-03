// Generic reset command handler for ARM Cortex-M boards
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "armcm_reset.h" // try_request_canboot
#include "autoconf.h" // CONFIG_FLASH_APPLICATION_ADDRESS
#include "board/internal.h" // NVIC_SystemReset
#include "board/irq.h" // irq_disable
#include "command.h" // DECL_COMMAND_FLAGS

#define CANBOOT_SIGNATURE 0x21746f6f426e6143
#define CANBOOT_REQUEST   0x5984E3FA6CA1589B
#define CANBOOT_BYPASS    0x7b06ec45a9a8243d

static void
canboot_reset(uint64_t req_signature)
{
    if (CONFIG_FLASH_APPLICATION_ADDRESS == CONFIG_FLASH_BOOT_ADDRESS)
        // No bootloader
        return;
    uint32_t *bl_vectors = (uint32_t *)CONFIG_FLASH_BOOT_ADDRESS;
    uint64_t *boot_sig = (uint64_t *)(bl_vectors[1] - 9);
    uint64_t *req_sig = (uint64_t *)bl_vectors[0];
    if (boot_sig != (void*)ALIGN((size_t)boot_sig, 8)
        || *boot_sig != CANBOOT_SIGNATURE
        || req_sig != (void*)ALIGN((size_t)req_sig, 8))
        return;
    irq_disable();
    *req_sig = req_signature;
#if __CORTEX_M >= 7 && (__CORTEX_M != 23)
    SCB_CleanDCache_by_Addr((void*)req_sig, sizeof(*req_sig));
#endif
    NVIC_SystemReset();
}

void
try_request_canboot(void)
{
    canboot_reset(CANBOOT_REQUEST);
}

void
command_reset(uint32_t *args)
{
    canboot_reset(CANBOOT_BYPASS);
    NVIC_SystemReset();
}
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "reset");
