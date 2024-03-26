// Support for SW and HW SPI commands to WS2812 type LEDs
//
// Copyright (C) 2024  Anders Törnberg <anders.tornberg@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "autoconf.h" // CONFIG_MACH_AVR
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_poll
#include "board/misc.h" // timer_read_time
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_shutdown
#include "spicmds.h" // spidev_se

/****************************************************************
 * Timing
 ****************************************************************/

typedef unsigned int ws2801_time_t;

static ws2801_time_t
nsecs_to_ticks(uint32_t ns)
{
    return timer_from_us(ns * 1000) / 1000000;
}

#define MIN_TICKS_BETWEEN_REQUESTS     nsecs_to_ticks(500000)

/****************************************************************
 * WS2801 interface
 ****************************************************************/

struct ws2801_s {
    struct spidev_s *spi;
    uint32_t last_req_time;
    uint16_t data_size;
    uint8_t data[0];
};

void
command_config_ws2801(uint32_t *args)
{
    uint16_t data_size = args[2];
    if (data_size & 0x8000)
        shutdown("Invalid ws2801 data_size");
    struct ws2801_s *ws = oid_alloc(args[0], command_config_ws2801
                                     , sizeof(*ws) + data_size);
    ws->spi = spidev_oid_lookup(args[1]);
    ws->data_size = data_size;
}
DECL_COMMAND(command_config_ws2801, "config_ws2801 oid=%c spi_oid=%c"
             " data_size=%hu");

static int
send_data(struct ws2801_s *ws)
{
    // Make sure the reset time has elapsed since last request
    uint32_t last_req_time = ws->last_req_time, \
             rmt = MIN_TICKS_BETWEEN_REQUESTS;
    uint32_t cur = timer_read_time();
    while (cur - last_req_time < rmt) {
        irq_poll();
        cur = timer_read_time();
    }

    // Transmit data
     uint8_t *data = ws->data;
     uint_fast16_t data_len = ws->data_size;
    spidev_transfer(ws->spi, 0, data_len, data);
    return 0;
}

void
command_ws2801_update(uint32_t *args)
{
    uint8_t oid = args[0];
    struct ws2801_s *ws = oid_lookup(oid, command_config_ws2801);
    uint_fast16_t pos = args[1];
    uint_fast8_t data_len = args[2];
    uint8_t *data = command_decode_ptr(args[3]);
    if (pos & 0x8000 || pos + data_len > ws->data_size)
        shutdown("Invalid ws2801 update command");
    memcpy(&ws->data[pos], data, data_len);
}
DECL_COMMAND(command_ws2801_update,
             "ws2801_update oid=%c pos=%hu data=%*s");

void
command_ws2801_send(uint32_t *args)
{
    uint8_t oid = args[0];
    struct ws2801_s *ws = oid_lookup(oid, command_config_ws2801);
    int ret = send_data(ws);
    sendf("ws2801_result oid=%c success=%c", oid, ret ? 0 : 1);
}
DECL_COMMAND(command_ws2801_send, "ws2801_send oid=%c");
