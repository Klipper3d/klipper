// Commands for sending messages to an uc1701 lcd driver
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//                     Eric Callahan <arksine.code@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_poll
#include "board/misc.h" // timer_from_us
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_SHUTDOWN

//#define UC1701_DEBUG

struct uc1701 {
    uint32_t last_cmd_time, cmd_wait_ticks;
    struct gpio_out sclk, sid, cs, a0;
#ifdef UC1701_DEBUG
    uint32_t cmd_bytes_recd, data_bytes_recd, debug_last_sent;
#endif
};

static void 
timer_delay(uint32_t delay_ticks) 
{
    uint32_t start = timer_read_time();
    while(timer_read_time() - start < delay_ticks)
        irq_poll();
}


/****************************************************************
 * Transmit functions
 ****************************************************************/

// Write eight bits to the uc1701 via the serial interface
static void
uc1701_xmit_byte(struct uc1701 *u, uint8_t data)
{
    struct gpio_out sclk = u->sclk, sid = u->sid;
    uint8_t i, sid_high=0;
    for (i=0; i<8; i++) {
        if (data & 0x80) {
            gpio_out_toggle(sid);
            sid_high = ~sid_high;
            data = ~data;
        }
        gpio_out_toggle(sclk);
        data <<= 1;
        gpio_out_toggle(sclk);
    }

    if (sid_high)
        gpio_out_toggle(sid);
}

// Transmit a series of bytes to the chip
static void
uc1701_xmit(struct uc1701 *u, uint8_t count, uint8_t *bytes)
{
    uint32_t last_cmd_time=u->last_cmd_time, cmd_wait_ticks=u->cmd_wait_ticks;
    while (count--) {
        uint8_t b = *bytes++;
        while (timer_read_time() - last_cmd_time < cmd_wait_ticks)
            irq_poll();
        uc1701_xmit_byte(u, b);
        last_cmd_time = timer_read_time();
    }
    u->last_cmd_time = last_cmd_time;
}

/****************************************************************
 * Interface
 ****************************************************************/

void
command_config_uc1701(uint32_t *args)
{
    struct uc1701 *u = oid_alloc(args[0], command_config_uc1701, sizeof(*u));
    u->cs = gpio_out_setup(args[1], 1);
    u->sclk = gpio_out_setup(args[2], 0);  
    u->sid = gpio_out_setup(args[3], 0);  
    u->a0 = gpio_out_setup(args[4], 0);

    irq_disable();
    uint32_t start = timer_read_time();
    uc1701_xmit_byte(u, 0xE3);  // NOP
    uint32_t end = timer_read_time();
    irq_enable();
    u->last_cmd_time = end;
    uint32_t diff = end - start, delay_ticks = args[5];
    if (delay_ticks > diff)
        u->cmd_wait_ticks = delay_ticks - diff;
#ifdef UC1701_DEBUG
    output("uc1701 configured, command_wait_ticks: %u", u->cmd_wait_ticks);
    u->cmd_bytes_recd = 0;
    u->data_bytes_recd = 0;
    u->debug_last_sent = 0;
#endif
}
DECL_COMMAND(command_config_uc1701,
             "config_uc1701 oid=%c cs_pin=%u sclk_pin=%u sid_pin=%u"
             " a0_pin=%u delay_ticks=%u");

void
command_uc1701_init(uint32_t *args)
{
    struct uc1701 *u = oid_lookup(args[0], command_config_uc1701);
#ifdef UC1701_DEBUG
    output("uc1701 initializing, oid: %u init array length: %u", args[0], args[1]);
#endif
    gpio_out_write(u->cs, 0);   // Enable Chip
    gpio_out_write(u->a0, 0);   // Enter Command mode
    uint8_t len = args[1], *data = (void*)(size_t)args[2];
    uint8_t display_all = 0xA5, normal_disp = 0xA4;
    uc1701_xmit(u, len, data);
    timer_delay(timer_from_us(100000));
    uc1701_xmit_byte(u, display_all);
    timer_delay(timer_from_us(200000));
    uc1701_xmit_byte(u, normal_disp);
    gpio_out_write(u->cs, 1);  // Disable Chip

}
DECL_COMMAND(command_uc1701_init, "uc1701_init oid=%c cmds=%*s");

void
command_uc1701_send_cmds(uint32_t *args)
{
    struct uc1701 *u = oid_lookup(args[0], command_config_uc1701);
    uint8_t len = args[1], *data = (void*)(size_t)args[2];
    gpio_out_write(u->cs, 0);
    gpio_out_write(u->a0, 0); 
    uc1701_xmit(u, len, data);
    gpio_out_write(u->cs, 1);
#ifdef UC1701_DEBUG
    u->cmd_bytes_recd += len;
#endif
}
DECL_COMMAND(command_uc1701_send_cmds, "uc1701_send_cmds oid=%c cmds=%*s");

void
command_uc1701_send_data(uint32_t *args)
{
    struct uc1701 *u = oid_lookup(args[0], command_config_uc1701);
    uint8_t len = args[1], *data = (void*)(size_t)args[2];
    gpio_out_write(u->cs, 0);
    gpio_out_write(u->a0, 1);   // Enter Data Mode
    uc1701_xmit(u, len, data);
    gpio_out_write(u->cs, 1);
#ifdef UC1701_DEBUG
    u->data_bytes_recd += len;
    if (timer_read_time() - u->debug_last_sent > 1000000) {
        output("Command Bytes Recd: %u, Data Bytes Recd: %u", u->cmd_bytes_recd, 
            u->data_bytes_recd);
        u->debug_last_sent = timer_read_time();
    }
#endif 
}
DECL_COMMAND(command_uc1701_send_data, "uc1701_send_data oid=%c data=%*s");

void
uc1701_shutdown(void)
{
    uint8_t i;
    struct uc1701 *u;
    foreach_oid(i, u, command_config_uc1701) {
        gpio_out_write(u->sclk, 0);
        gpio_out_write(u->sid, 0);
        gpio_out_write(u->cs, 0);  
        gpio_out_write(u->a0, 0);
    }
}
DECL_SHUTDOWN(uc1701_shutdown);
