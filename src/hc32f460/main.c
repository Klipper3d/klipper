// Code to setup clocks on Huada HC32F460
//
// Copyright (C) 2022 Steven Gotthardt
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "sched.h"

#include "hc32_ddl.h"

// CPU LED @ PA3    - use for debug or task
#define  LED0_PORT        (PortA)
#define  LED0_PIN         (Pin03)

void
led_TOGGLE(void)
{
    PORT_Toggle(LED0_PORT, LED0_PIN);
}
DECL_TASK(led_TOGGLE);

void led_ON(void)  { PORT_ResetBits(LED0_PORT, LED0_PIN); }
void led_OFF(void) { PORT_SetBits(LED0_PORT, LED0_PIN); }



/****************************************************************
 * Startup
 ****************************************************************/

// Main entry point - called from armcm_boot.c:ResetHandler()
void  __attribute__((noreturn))
armcm_main(void)
{
    // setup the 'CPU' LED - active low
    stc_port_init_t stcPortInit = {0};
    stcPortInit.enPinMode = Pin_Mode_Out;
    PORT_Init(LED0_PORT, LED0_PIN, &stcPortInit);

    // sets the system clock speed variable for library use
    SystemInit();

    // manage the system
    sched_main();

    // never get here
    for (;;)  ;
}
