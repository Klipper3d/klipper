// Main starting point for STM32 boards.
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "internal.h" // clock_setup
#include "sched.h" // sched_main

// Main entry point
int
main(void)
{
    clock_setup();

    sched_main();
    return 0;
}
