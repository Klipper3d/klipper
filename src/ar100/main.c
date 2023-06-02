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
irq_poll(void)
{
    if(timer_interrupt_pending()) {
        timer_clear_interrupt();
        uint32_t next = timer_dispatch_many();
        timer_set(next);
    }
    if(r_uart_fifo_rcv())
        sched_wake_task(&console_wake);
}

/****************************************************************
* Console IO
****************************************************************/

// Process any incoming commands
void
console_task(void)
{
    if (!sched_check_wake(&console_wake))
        return;

    int ret = 0;
    for(int i=0; i<r_uart_fifo_rcv(); i++) {
        receive_buf[receive_pos + ret++] = r_uart_getc();
    }
    if(!ret)
        return;

    int len = receive_pos + ret;
    uint_fast8_t pop_count, msglen = len > MESSAGE_MAX ? MESSAGE_MAX : len;
    ret = command_find_and_dispatch(receive_buf, msglen, &pop_count);
    if (ret) {
        len -= pop_count;
        if (len) {
            memcpy(receive_buf, &receive_buf[pop_count], len);
            sched_wake_task(&console_wake);
        }
    }
    receive_pos = len;
}
DECL_TASK(console_task);

// Encode and transmit a "response" message
void
console_sendf(const struct command_encoder *ce, va_list args)
{
    uint8_t buf[MESSAGE_MAX];
    uint_fast8_t msglen = command_encode_and_frame(buf, ce, args);

    for(int i=0; i<msglen; i++) {
        r_uart_putc(buf[i]);
    }
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
