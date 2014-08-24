// Main starting point for AVR boards.
//
// Copyright (C) 2014  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#include "irq.h" // irq_enable
#include "sched.h" // sched_kick

// Main entry point for avr code.
int
main(void)
{
    irq_enable();
    sched_kick();
    return 0;
}
