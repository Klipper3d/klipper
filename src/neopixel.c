// Support for bit-banging commands to WS2812 type "neopixel" LEDs
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_MACH_AVR
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_poll
#include "board/misc.h" // timer_read_time
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND

// The WS2812 uses a bit-banging protocol where each bit is
// transmitted as a gpio high pulse of variable length.  The various
// specs are unclear, but it is believed the timing requirements are:
// - A zero bit must have a high pulse less than 500ns.
// - A one bit must have a high pulse longer than 650ns.
// - The total bit time (gpio high to following gpio high) must not
//   exceed ~5000ns. The average bit time must be at least 1250ns.
// - The specs generally indicate a minimum high pulse and low pulse
//   of 200ns, but the actual requirement might be smaller.



/****************************************************************
 * Timing
 ****************************************************************/

typedef unsigned int neopixel_time_t;

static neopixel_time_t
nsecs_to_ticks(uint32_t ns)
{
    return timer_from_us(ns * 1000) / 1000000;
}

static inline int
neopixel_check_elapsed(neopixel_time_t t1, neopixel_time_t t2
                       , neopixel_time_t ticks)
{
    return t2 - t1 >= ticks;
}

// The AVR micro-controllers require specialized timing
#if CONFIG_MACH_AVR

#include <avr/interrupt.h> // TCNT1

static neopixel_time_t
neopixel_get_time(void)
{
    return TCNT1;
}

#define neopixel_delay(start, ticks) (void)(ticks)

#else

static neopixel_time_t
neopixel_get_time(void)
{
    return timer_read_time();
}

static inline void
neopixel_delay(neopixel_time_t start, neopixel_time_t ticks)
{
    while (!neopixel_check_elapsed(start, neopixel_get_time(), ticks))
        ;
}

#endif

#define PULSE_LONG_TICKS  nsecs_to_ticks(650)
#define PULSE_SHORT_TICKS nsecs_to_ticks(200)
#define BIT_MIN_TICKS     nsecs_to_ticks(1250)


/****************************************************************
 * Neopixel interface
 ****************************************************************/

struct neopixel_s {
    struct gpio_out pin;
    neopixel_time_t bit_max_ticks;
    uint32_t last_req_time, reset_min_ticks;
    uint32_t repeat_len, repeat_count;
};

void
command_config_neopixel(uint32_t *args)
{
    struct gpio_out pin = gpio_out_setup(args[1], 0);
    struct neopixel_s *n = oid_alloc(args[0], command_config_neopixel
                                     , sizeof(*n));
    n->pin = pin;
    n->bit_max_ticks = args[2];
    n->reset_min_ticks = args[3];
    n->repeat_len = args[4];
    n->repeat_count = args[5];
}
DECL_COMMAND(command_config_neopixel, "config_neopixel oid=%c pin=%u"
             " bit_max_ticks=%u reset_min_ticks=%u"
             " repeat_len=%u repeat_count=%u");

static int
send_byte(struct neopixel_s *n, neopixel_time_t *last_start, uint8_t byte)
{
    struct gpio_out pin = n->pin;
    const neopixel_time_t bit_max_ticks = n->bit_max_ticks;

    for (uint_fast8_t i = 0; i < 8; i++) {
        neopixel_time_t start;

        neopixel_delay(*last_start, BIT_MIN_TICKS);
        if (byte & 0x80) {
            // Long pulse
            irq_disable();
            start = neopixel_get_time();
            gpio_out_toggle_noirq(pin);
            irq_enable();

            if (neopixel_check_elapsed(*last_start, start, bit_max_ticks))
                return -1;
            neopixel_delay(start, PULSE_LONG_TICKS);

            irq_disable();
            gpio_out_toggle_noirq(pin);
            irq_enable();
        } else {
            // Short pulse
            irq_disable();
            start = neopixel_get_time();
            gpio_out_toggle_noirq(pin);
            neopixel_delay(start, PULSE_SHORT_TICKS);
            gpio_out_toggle_noirq(pin);
            irq_enable();

            if (neopixel_check_elapsed(*last_start, start, bit_max_ticks))
                return -1;
        }

        *last_start = start;
        byte <<= 1;
    }
    return 0;
}

static int
send_data(struct neopixel_s *n, uint8_t *data, uint_fast8_t data_len)
{
    // Make sure the reset time has elapsed since last request
    uint32_t last_req_time = n->last_req_time, rmt = n->reset_min_ticks;
    uint32_t cur = timer_read_time();
    while (cur - last_req_time < rmt) {
        irq_poll();
        cur = timer_read_time();
    }

    // Transmit data
    neopixel_time_t last_start = neopixel_get_time();

    for (uint8_t i = 0; i < data_len; i += n->repeat_len) {
        for (uint8_t repeat = 0; repeat < n->repeat_count; repeat++) {
            for (uint8_t j = 0; j < n->repeat_len && i + j < data_len; j++) {
                if (send_byte(n, &last_start, data[i + j]))
                    goto fail;
            }
        }
    }
    n->last_req_time = timer_read_time();
    return 0;
fail:
    // A hardware irq messed up the transmission - report a failure
    gpio_out_write(n->pin, 0);
    n->last_req_time = timer_read_time();
    return -1;
}

void
command_neopixel_send(uint32_t *args)
{
    uint8_t oid = args[0];
    struct neopixel_s *n = oid_lookup(oid, command_config_neopixel);
    uint_fast8_t data_len = args[1];
    uint8_t *data = (void*)(size_t)args[2];

    uint_fast8_t retry = 8;
    while (retry--) {
        int ret = send_data(n, data, data_len);
        if (!ret)
            break;
    }
}
DECL_COMMAND(command_neopixel_send, "neopixel_send oid=%c data=%*s");
