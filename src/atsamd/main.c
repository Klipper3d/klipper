// Main starting point for SAMD boards
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "sched.h" // sched_main

// Main entry point
int
main(void)
{
    sched_main();
    return 0;
}
