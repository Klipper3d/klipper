// Code to setup clocks on Huada HC32F460
//
// Copyright (C) 2022  Steven Gotthardt <gotthardt@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "sched.h"
#include "hc32f460_gpio.h"


// CPU LED @ PA3    - use for debug or task
#define  CPULED_PORT    (PortA)
#define  CPULED_PIN     (Pin03)

void
led_TOGGLE(void)
{
    PORT_Toggle(CPULED_PORT, CPULED_PIN);
}
DECL_TASK(led_TOGGLE);


void
led_init(void)
{
    // setup the 'CPU' LED - active low
    stc_port_init_t stcPortInit = {0};
    stcPortInit.enPinMode = Pin_Mode_Out;
    PORT_Init(CPULED_PORT, CPULED_PIN, &stcPortInit);
}
DECL_INIT(led_init);



/****************************************************************
 * Startup
 ****************************************************************/

// Main entry point - called from armcm_boot.c:ResetHandler()
void  __attribute__((noreturn))
armcm_main(void)
{
    // sets the system clock speed variable for library use
    SystemInit();

    // manage the system
    sched_main();

    // never get here
    for (;;)  ;
}
