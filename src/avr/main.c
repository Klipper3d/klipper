// Main starting point for AVR boards.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "irq.h" // irq_enable
#include "sched.h" // sched_main

// Main entry point for avr code.
int
main(void)
{
    irq_enable();
    sched_main();
    return 0;
}
