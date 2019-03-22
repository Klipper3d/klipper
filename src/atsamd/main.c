// Main starting point for SAMD boards
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // DECL_CONSTANT
#include "internal.h" // NVIC_SystemReset
#include "sched.h" // sched_main

DECL_CONSTANT_STR("MCU", CONFIG_MCU);

// Return the start of memory available for dynamic allocations
void *
dynmem_start(void)
{
    extern uint32_t _ezero;
    return &_ezero;
}

// Return the end of memory available for dynamic allocations
void *
dynmem_end(void)
{
    extern uint32_t _sstack;
    return &_sstack;
}

void
command_reset(uint32_t *args)
{
    NVIC_SystemReset();
}
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "reset");

// Main entry point
int
main(void)
{
    SystemInit();
    sched_main();
    return 0;
}
