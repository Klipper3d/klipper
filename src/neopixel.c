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
#if !CONFIG_MACH_AVR
DECL_COMMAND(command_config_neopixel, "config_neopixel oid=%c pin=%u");
#endif

static uint32_t
timer_from_ns(uint32_t ns)
{
    return timer_from_us(ns * 1000) / 1000000;
}

// The WS2812 uses a bit-banging protocol where each bit is
// transmitted as a gpio high pulse of variable length.  The various
// specs are unclear, but it is believed the timing requirements are:
// - A zero bit must have a high pulse less than 500ns.
// - A one bit must have a high pulse longer than 650ns.
// - The total bit time (gpio high to following gpio high) must not
//   exceed ~5000ns. The average bit time must be at least 1250ns.
// - The specs generally indicate a minimum high pulse and low pulse
//   of 200ns, but the actual requirement might be smaller.

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
    uint32_t min_wait_time = cur, max_wait_time = cur + 0x40000000;
    while (data_len--) {
        uint_fast8_t byte = *data++;
        uint_fast8_t bits = 8;
        while (bits--) {
            // Calculate pulse duration
            uint32_t on;
            if (byte & 0x80)
                on = timer_from_ns(650);
            else
                on = timer_from_ns(200);

            // Set output high
            do {
                irq_poll();
                cur = timer_read_time();
            } while (timer_is_before(cur, min_wait_time));
            uint32_t off_end_time = cur;
            gpio_out_write(pin, 1);
            uint32_t on_start_time = timer_read_time();
            min_wait_time = on_start_time + on;

            // Set output low
            do {
                irq_poll();
                cur = timer_read_time();
            } while (timer_is_before(cur, min_wait_time));
            uint32_t on_end_time = cur;
            gpio_out_write(pin, 0);
            uint32_t off_start_time = cur = timer_read_time();
            min_wait_time = on_start_time + timer_from_ns(1250);

            // Check for faults
            if (byte & 0x80) {
                // Make sure off for at least 200ns
                uint32_t min_off = off_start_time + timer_from_ns(200);
                if (timer_is_before(min_wait_time, min_off))
                    min_wait_time = min_off;
            } else {
                // Make sure short on duration was no more than 500ns
                uint32_t max_off = off_end_time + timer_from_ns(500);
                if (timer_is_before(max_off, off_start_time))
                    goto fail;
            }
            byte <<= 1;
            if (timer_is_before(max_wait_time, on_start_time))
                goto fail;
            max_wait_time = on_end_time + timer_from_us(4);
        }
    }
    n->last_req_time = cur;
    return 0;
fail:
    // A hardware irq messed up the transmission - report a failure
    n->last_req_time = cur;
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
#if !CONFIG_MACH_AVR
DECL_COMMAND(command_neopixel_send, "neopixel_send oid=%c data=%*s");
#endif
