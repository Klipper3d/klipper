// Main entry point for ar100
//
// Copyright (C) 2020-2021  Elias Bakken <elias@iagent.no>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h>     // uint32_t
#include <string.h>
#include "board/misc.h" // dynmem_start
#include "board/irq.h"  // irq_disable
#include "command.h"    // shutdown
#include "generic/serial_irq.h" // serial_enable_tx_irq
#include "generic/timer_irq.h"  // timer_dispatch_many
#include "sched.h"      // sched_main

#include "asm/spr.h"
#include "util.h"
#include "gpio.h"
#include "serial.h"
#include "timer.h"

DECL_CONSTANT_STR("MCU", "ar100");

#define RESET_VECTOR 0x0100

static struct task_wake console_wake;
static uint8_t receive_buf[192];
static int receive_pos;
static char dynmem_pool[8 * 1024];

void *
dynmem_start(void)
{
    return dynmem_pool;
}

void *
dynmem_end(void)
{
    return &dynmem_pool[sizeof(dynmem_pool)];
}

static int need_serial_tx;

static void
check_serial_pending(void)
{
    while (r_uart_fifo_rcv())
        serial_rx_byte(r_uart_getc());
    while (need_serial_tx && r_uart_fifo_cantx()) {
        uint8_t b;
        int ret = serial_get_tx_byte(&b);
        if (ret < 0)
            need_serial_tx = 0;
        else
            r_uart_putc(b);
    }
}

void
serial_enable_tx_irq(void)
{
    need_serial_tx = 1;
    check_serial_pending();
}

void
irq_disable(void)
{
}

void
irq_enable(void)
{
}

irqstatus_t
irq_save(void)
{
    return 0;
}

void
irq_restore(irqstatus_t flag)
{
}

void
irq_wait(void)
{
    irq_poll();
}

void
timer_dispatch_irq_poll(void)
{
}

void
timer_dispatch_task_poll(void)
{
    check_serial_pending();
}

void
irq_poll(void)
{
    if(timer_interrupt_pending()) {
        timer_clear_interrupt();
        uint32_t next = timer_dispatch_many_polling();
        timer_set(next);
    }
    check_serial_pending();
}

void restore_data(void)
{
    extern char __data_start, __data_end, __copy_start;
    memcpy (&__data_start, &__copy_start, &__data_end - &__data_start);
}

void
command_reset(uint32_t *args)
{
    timer_reset();
    restore_data();
    void *reset = (void *)RESET_VECTOR;
    goto *reset;
}
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "reset");

void
save_data(void)
{
    extern char __data_start, __data_end, __copy_start;
    memcpy (&__copy_start, &__data_start, &__data_end - &__data_start);
}

__noreturn void
main(uint32_t exception);
__noreturn void
main(uint32_t exception)
{
    save_data();
    r_uart_init();
    sched_main();
    while(1) {}         // Stop complaining about noreturn
}
