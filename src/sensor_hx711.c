// Support for hx711 load cell sensor
//
// Copyright (C) 2022  Jochen Loeser <jochen_loeser@trimble.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_MACH_AVR
#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_from_us
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK

enum hx_state {
    HX_IDLE,
    HX_START,
    HX_VALIDATE,
    HX_READING,
    HX_DONE,
};

struct hx711 {
    struct gpio_in  dout_pin;   // pin used to receive data from the hx711
    struct gpio_out sclk_pin;   // pin used to generate clock for the hx711
    struct timer    timer;      // time to trigger hx711_task()
    uint32_t        rest_ticks; //
    enum hx_state   state;      // contains the current state
    int32_t         data;       // received data
};

static struct task_wake hx711_wake;

// Event handler that wakes hx711_task() periodically
static uint_fast8_t
hx711_event(struct timer *timer)
{
    sched_wake_task(&hx711_wake);
    return SF_DONE;
}

static uint32_t
nsecs_to_ticks(uint32_t ns)
{
    return timer_from_us(ns * 1000) / 1000000;
}

static inline void
ndelay(uint32_t nsecs)
{
    if (CONFIG_MACH_AVR)
    {
        // Slower MCUs don't require a delay
        return;
    }
    uint32_t end = timer_read_time() + nsecs_to_ticks(nsecs);
    while (timer_is_before(timer_read_time(), end))
    {
        irq_poll();
    }
}

static inline uint32_t
hx711_read_dout(struct hx711 *hx)
{
    return gpio_in_read(hx->dout_pin);
}

static inline void
hx711_set_sclk(struct hx711 *hx, uint32_t value)
{
    gpio_out_write(hx->sclk_pin, value);
}

static inline uint32_t
hx711_receive_bit(struct hx711 *hx)
{
    hx711_set_sclk(hx, 1);
    ndelay(250);
    uint32_t result = hx711_read_dout(hx);
    hx711_set_sclk(hx, 0);
    ndelay(250);
    return result;
}

static inline uint32_t
hx711_rdy(struct hx711 *hx)
{
    return !hx711_read_dout(hx);
}

void
command_config_hx711(uint32_t *args)
{
    struct hx711 *hx = oid_alloc(args[0], command_config_hx711, sizeof(*hx));
    hx->sclk_pin = gpio_out_setup(args[1], 0);
    hx->dout_pin = gpio_in_setup(args[2], 0);
    hx->timer.func = hx711_event;
    hx->state = HX_IDLE;
    hx->data = 0;
}
DECL_COMMAND(command_config_hx711,
    "config_hx711 oid=%c sclk_pin=%u dout_pin=%u");

// Helper code to reschedule the hx711_event() timer
static void
hx711_reschedule_timer(struct hx711 *hx)
{
    irq_disable();
    sched_del_timer(&hx->timer);
    hx->timer.waketime = timer_read_time() + hx->rest_ticks;
    sched_add_timer(&hx->timer);
    irq_enable();
}

void
command_query_hx711_value(uint32_t *args)
{
    struct hx711 *hx = oid_lookup(args[0], command_config_hx711);
    if(hx->state == HX_IDLE)
    {
        irq_disable();
        if(hx->state == HX_IDLE)
        {
            sched_del_timer(&hx->timer);
            hx->timer.waketime = args[1];
            hx->rest_ticks = args[2];
            hx->state = HX_START;
            sched_add_timer(&hx->timer);
        }
        irq_enable();
    }
}
DECL_COMMAND(command_query_hx711_value,
    "query_hx711_value oid=%c clock=%u rest_ticks=%u");

void
hx711_task(void)
{
    if (!sched_check_wake(&hx711_wake))
    {
        return;
    }
    uint8_t oid;
    struct hx711 *hx;
    foreach_oid(oid, hx, command_config_hx711) {
        switch(hx->state)
        {
            case HX_IDLE:
                return;
            case HX_START:
                hx711_set_sclk(hx, 0);
                hx->data = 0;
                hx->state = HX_VALIDATE;
                hx711_reschedule_timer(hx);
                break;
            case HX_VALIDATE:
                if(hx711_rdy(hx))
                {
                    hx->state = HX_READING;
                }
                hx711_reschedule_timer(hx);
                break;
            case HX_READING:
                for(uint32_t i = 0; i < 24; i++)
                {
                    hx->data <<=1;
                    hx->data |= hx711_receive_bit(hx);
                }
                if(hx->data & 0x00800000)
                {
                    hx->data |= 0xFF000000;
                }
                hx->state = HX_DONE;
                hx711_reschedule_timer(hx);
                break;
            case HX_DONE:
                sendf("hx711_value oid=%c value=%i", oid, hx->data);
                hx711_receive_bit(hx);
                hx711_set_sclk(hx, 1);
                hx->state = HX_IDLE;
                break;
        }
    }
}
DECL_TASK(hx711_task);
