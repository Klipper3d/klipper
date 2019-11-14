// Main starting point for SAMD boards
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/armcm_boot.h" // armcm_main
#include "internal.h" // SystemInit
#include "sched.h" // sched_main

// Main entry point - called from armcm_boot.c:ResetHandler()
void
armcm_main(void)
{
    SystemInit();
    sched_main();
}
