// Support for serial port bridging
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "autoconf.h" // CONFIG_MACH_AVR
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_poll
#include "board/misc.h" // timer_read_time
#include "board/io.h" // readb
#include "generic/serial_bridge_irq.h" // console2_sendf
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_shutdown

struct serial_bridge {
    struct timer timer;
    uint32_t rest_time;
};

static struct task_wake serial_bridge_wake;

static uint_fast8_t serial_bridge_event(struct timer *timer) {
    struct serial_bridge *bridge = container_of(
            timer, struct serial_bridge, timer);

    sched_wake_task(&serial_bridge_wake);

    bridge->timer.waketime += bridge->rest_time;

    return SF_RESCHEDULE;
}

void
command_config_serial_bridge(uint32_t *args)
{
    struct serial_bridge *bridge = oid_alloc(
        args[0], command_config_serial_bridge, sizeof(*bridge));
    bridge->timer.func = serial_bridge_event;
    bridge->timer.waketime = args[1];
    bridge->rest_time = args[2];

    sched_add_timer(&bridge->timer);
}
DECL_COMMAND(command_config_serial_bridge,
             "command_config_serial_bridge oid=%c clock=%u rest_ticks=%u");

void
command_serial_bridge_send(uint32_t *args)
{
    uint8_t data_len = args[1];
    uint8_t *data = command_decode_ptr(args[2]);

    //output("sb %s", data);

    serial_bridge_send(data, data_len);
}
DECL_COMMAND(command_serial_bridge_send, "serial_bridge_send oid=%c text=%*s");

void
command_serial_bridge_stats(uint32_t *args)
{
    output("bridge_stats: %u/%u", 0, 0);
}
DECL_COMMAND(command_serial_bridge_stats, "serial_bridge_stats");

void
serial_bridge_task(void)
{
    if (!sched_check_wake(&serial_bridge_wake))
        return;

    static uint8_t buf[SERIAL_BRIDGE_RX_BUFFER_SIZE];

    unsigned int data_len = serial_bridge_get_data(buf);
    if (data_len) {
        sendf("serial_bridge_response text=%*s", data_len, buf);
    }
}
DECL_TASK(serial_bridge_task);