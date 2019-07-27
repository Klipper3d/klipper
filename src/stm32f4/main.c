// Main starting point for STM32F4 boards.
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_MCU
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // clock_setup
#include "sched.h" // sched_main

DECL_CONSTANT_STR("MCU", CONFIG_MCU);

// Return the start of memory available for dynamic allocations
void *
dynmem_start(void)
{
    extern uint32_t _ebss;
    return &_ebss;
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
    clock_setup();

    sched_main();
    return 0;
}
