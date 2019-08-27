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
neopixel_check_elapsed(neopixel_time_t t1, neopixel_time_t t2, uint32_t nsecs)
{
    return t2 - t1 >= nsecs_to_ticks(nsecs);
}

// The AVR micro-controllers require specialized timing
#if CONFIG_MACH_AVR

#include <avr/interrupt.h> // TCNT1

static neopixel_time_t
neopixel_get_time(void)
{
    return TCNT1;
}

static inline void
neopixel_delay(neopixel_time_t start, uint32_t nsecs)
{
}

#else

static neopixel_time_t
neopixel_get_time(void)
{
    return timer_read_time();
}

static inline void
neopixel_delay(neopixel_time_t start, uint32_t nsecs)
{
    while (!neopixel_check_elapsed(start, neopixel_get_time(), nsecs))
        ;
}

#endif


/****************************************************************
 * Neopixel interface
 ****************************************************************/

struct neopixel_s {
    struct gpio_out pin;
    uint32_t last_req_time;
};

void
command_config_neopixel(uint32_t *args)
{
    struct gpio_out pin = gpio_out_setup(args[1], 0);
    struct neopixel_s *n = oid_alloc(args[0], command_config_neopixel
                                     , sizeof(*n));
    n->pin = pin;
}
DECL_COMMAND(command_config_neopixel, "config_neopixel oid=%c pin=%u");

static int
send_data(struct neopixel_s *n, uint8_t *data, uint_fast8_t data_len)
{
    // Make sure at least 50us has passed since last request
    uint32_t last_req_time = n->last_req_time, cur = timer_read_time();
    while (cur - last_req_time < timer_from_us(50)) {
        irq_poll();
        cur = timer_read_time();
    }

    struct gpio_out pin = n->pin;
    neopixel_time_t last_start = neopixel_get_time();
    while (data_len--) {
        uint_fast8_t byte = *data++;
        uint_fast8_t bits = 8;
        while (bits--) {
            if (byte & 0x80) {
                // Long pulse
                neopixel_delay(last_start, 1250);
                irq_disable();
                neopixel_time_t start = neopixel_get_time();
                gpio_out_toggle_noirq(pin);
                irq_enable();

                if (neopixel_check_elapsed(last_start, start, 4000))
                    goto fail;
                last_start = start;
                byte <<= 1;

                neopixel_delay(start, 650);
                irq_disable();
                gpio_out_toggle_noirq(pin);
                irq_enable();
            } else {
                // Short pulse
                neopixel_delay(last_start, 1250);
                irq_disable();
                neopixel_time_t start = neopixel_get_time();
                gpio_out_toggle_noirq(pin);
                neopixel_delay(start, 200);
                gpio_out_toggle_noirq(pin);
                irq_enable();

                if (neopixel_check_elapsed(last_start, start, 4000))
                    goto fail;
                last_start = start;
                byte <<= 1;
            }
        }
    }
    n->last_req_time = timer_read_time();
    return 0;
fail:
    // A hardware irq messed up the transmission - report a failure
    gpio_out_write(pin, 0);
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
