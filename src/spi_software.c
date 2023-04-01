// Software SPI emulation
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/gpio.h" // gpio_out_setup
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_shutdown
#include "spicmds.h" // spidev_set_software_bus
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_from_us
#include "autoconf.h" // CONFIG_MACH_AVR

const uint32_t NSECS_IN_HZ = 1000000000;

struct spi_software {
    struct gpio_in miso;
    struct gpio_out mosi, sclk;
    uint8_t mode;
    uint32_t delay;
};


static uint32_t
nsecs_to_ticks(uint32_t ns)
{
    return timer_from_us(ns * 1000) / 1000000;
}

static inline void
ndelay(uint32_t nsecs)
{
    if (CONFIG_MACH_AVR)
        // Slower MCUs don't require a delay
        return;
    uint32_t end = timer_read_time() + nsecs_to_ticks(nsecs);
    while (timer_is_before(timer_read_time(), end))
        irq_poll();
}

void
command_spi_set_software_bus(uint32_t *args)
{
    uint8_t mode = args[4];
    if (mode > 3)
        shutdown("Invalid spi config");

    struct spidev_s *spi = spidev_oid_lookup(args[0]);
    struct spi_software *ss = alloc_chunk(sizeof(*ss));
    ss->miso = gpio_in_setup(args[1], 1);
    ss->mosi = gpio_out_setup(args[2], 0);
    ss->sclk = gpio_out_setup(args[3], 0);
    ss->mode = mode;
    ss->delay = NSECS_IN_HZ * 1/2 * args[5]; //convert to nsecs
    spidev_set_software_bus(spi, ss);
}
DECL_COMMAND(command_spi_set_software_bus,
             "spi_set_software_bus oid=%c miso_pin=%u mosi_pin=%u sclk_pin=%u"
             " mode=%u rate=%u");

void
spi_software_prepare(struct spi_software *ss)
{
    gpio_out_write(ss->sclk, ss->mode & 0x02);
}

void
spi_software_transfer(struct spi_software *ss, uint8_t receive_data
                      , uint8_t len, uint8_t *data)
{
    while (len--) {
        uint8_t outbuf = *data;
        uint8_t inbuf = 0;
        for (uint_fast8_t i = 0; i < 8; i++) {
            if (ss->mode & 0x01) {
                // MODE 1 & 3
                gpio_out_toggle(ss->sclk);
                gpio_out_write(ss->mosi, outbuf & 0x80);
                outbuf <<= 1;
                ndelay(ss->delay);
                gpio_out_toggle(ss->sclk);
                ndelay(ss->delay);
                inbuf <<= 1;
                inbuf |= gpio_in_read(ss->miso);
            } else {
                // MODE 0 & 2
                gpio_out_write(ss->mosi, outbuf & 0x80);
                outbuf <<= 1;
                gpio_out_toggle(ss->sclk);
                ndelay(ss->delay);
                inbuf <<= 1;
                inbuf |= gpio_in_read(ss->miso);
                gpio_out_toggle(ss->sclk);
                ndelay(ss->delay);
            }
        }

        if (receive_data)
            *data = inbuf;
        data++;
    }
}
