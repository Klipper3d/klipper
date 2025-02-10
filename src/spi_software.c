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

struct spi_software {
    struct gpio_in miso;
    struct gpio_out mosi, sclk;
    uint8_t mode;
    uint8_t width;
};

void
command_spi_set_software_bus(uint32_t *args)
{
    uint8_t mode = args[4];
    uint8_t width = args[6];
    if (mode > 3 || width < 5 || width > 64) {
        output("command_spi_set_software_bus width=%c mode=%c", width, mode);
        shutdown("Invalid spi config");
    }

    struct spidev_s *spi = spidev_oid_lookup(args[0]);
    struct spi_software *ss = alloc_chunk(sizeof(*ss));
    ss->miso = gpio_in_setup(args[1], 1);
    ss->mosi = gpio_out_setup(args[2], 0);
    ss->sclk = gpio_out_setup(args[3], 0);
    ss->mode = mode;
    ss->width = width;
    spidev_set_software_bus(spi, ss);
}
DECL_COMMAND(command_spi_set_software_bus,
             "spi_set_software_bus oid=%c miso_pin=%u mosi_pin=%u sclk_pin=%u"
             " mode=%u rate=%u width=%u");

void
spi_software_prepare(struct spi_software *ss)
{
    gpio_out_write(ss->sclk, ss->mode & 0x02);
}

void
spi_software_transfer(struct spi_software *ss, uint8_t receive_data
                      , uint8_t len, uint8_t *data)
{
    uint16_t num_values = (uint16_t)len * 8 / ss->width;
    uint8_t  extra_bits = (uint16_t)len * 8 % ss->width;

    if (num_values < 1) {
        output("spi_software_transfer width=%c len=%c num_values=%hu, " \
               "extra_bits=%c", ss->width, len, num_values, extra_bits);
        shutdown("Invalid spi transfer: not enough data for at least 1 value");
    }

    if (extra_bits > 7) {
        output("spi_software_transfer width=%c len=%c num_values=%hu, " \
               "extra_bits=%c", ss->width, len, num_values, extra_bits);
        shutdown("Invalid spi transfer: unused bytes at the end of data");
    }

    while (len--) {
        uint8_t outbuf = *data;
        uint8_t inbuf = 0;
        uint_fast8_t end = (len) ? 8 : 8 - extra_bits;
        for (uint_fast8_t i = 0; i < end; i++) {
            if (ss->mode & 0x01) {
                // MODE 1 & 3
                gpio_out_toggle(ss->sclk);
                gpio_out_write(ss->mosi, outbuf & 0x80);
                outbuf <<= 1;
                gpio_out_toggle(ss->sclk);
                inbuf <<= 1;
                inbuf |= gpio_in_read(ss->miso);
            } else {
                // MODE 0 & 2
                gpio_out_write(ss->mosi, outbuf & 0x80);
                outbuf <<= 1;
                gpio_out_toggle(ss->sclk);
                inbuf <<= 1;
                inbuf |= gpio_in_read(ss->miso);
                gpio_out_toggle(ss->sclk);
            }
        }

        if (receive_data)
            *data = inbuf;
        data++;
    }
}
