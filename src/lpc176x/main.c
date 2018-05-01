// Main starting point for LPC176x boards.
//
// Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"
#include "command.h" // DECL_CONSTANT
#include "sched.h" // sched_main
#include "board/irq.h"  // irq_save

#include <system_LPC17xx.h>
#include <lpc17xx_clkpwr.h>

// debug:
#include "pins_MKS.h"
#include "delay.h"

extern void serial_init(void);


void kill_loop(void) {
    irq_disable();
    while(1) {
        gpio_out_write(SBASE_LED0, 1);
        gpio_out_write(SBASE_LED1, 1);
        gpio_out_write(SBASE_LED2, 1);
        gpio_out_write(SBASE_LED3, 1);
        gpio_out_write(SBASE_LED4, 1);
        DELAY_ms(500);
        gpio_out_write(SBASE_LED0, 0);
        gpio_out_write(SBASE_LED1, 0);
        gpio_out_write(SBASE_LED2, 0);
        gpio_out_write(SBASE_LED3, 0);
        gpio_out_write(SBASE_LED4, 0);
        DELAY_ms(500);
    }
}

//void NMI_Handler(void)        { kill_loop(); }
void HardFault_Handler(void)  { kill_loop(); }
void MemManage_Handler(void)  { kill_loop(); }
void BusFault_Handler(void)   { kill_loop(); }
void UsageFault_Handler(void) { kill_loop(); }

DECL_CONSTANT(MCU, "lpc176x");

struct gpio_out SBASE_LED0;
struct gpio_out SBASE_LED1;
struct gpio_out SBASE_LED2;
struct gpio_out SBASE_LED3;
struct gpio_out SBASE_LED4;

/****************************************************************
 * misc functions
 ****************************************************************/

void
command_reset(uint32_t *args)
{
    (void)args;
    NVIC_SystemReset();
}
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "reset");

// Main entry point
int main(void) {
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPIO, ENABLE);

    // Switch off the MKS SBase leds
    SBASE_LED0 = gpio_out_setup(pin_SBASE_LED0, 0);
    SBASE_LED1 = gpio_out_setup(pin_SBASE_LED1, 0);
    SBASE_LED2 = gpio_out_setup(pin_SBASE_LED2, 0);
    SBASE_LED3 = gpio_out_setup(pin_SBASE_LED3, 0);
    SBASE_LED4 = gpio_out_setup(pin_SBASE_LED4, 0);

    serial_uart_init();
    serial_init();

    serial_uart_printf("CPU freq: %d\n", SystemCoreClock);
    serial_uart_printf("Clock freq: %d\n", CONFIG_CLOCK_FREQ);

    gpio_out_write(SBASE_LED0, 0);
    gpio_out_write(SBASE_LED1, 0);
    gpio_out_write(SBASE_LED2, 0);
    gpio_out_write(SBASE_LED3, 0);
    gpio_out_write(SBASE_LED4, 0);

    sched_main();
    return 0;
}
