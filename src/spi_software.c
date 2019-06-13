// Software SPI emulation
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // gpio_out_setup
#include "board/gpio.h" // gpio_out_setup
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_shutdown

struct spi_software {
    struct gpio_out sclk, mosi;
    struct gpio_in miso;
    uint8_t mode;
};

void
command_config_software_spi(uint32_t *args)
{
    uint8_t oid = args[0], sclk_pin = args[1], mosi_pin = args[2];
    uint8_t miso_pin = args[3], mode = args[4];
    if (mode > 3)
        shutdown("Invalid spi mode");

    struct spi_software *spi = oid_alloc(oid, command_config_software_spi
                                         , sizeof(*spi));

    spi->sclk = gpio_out_setup(sclk_pin, 0);
    spi->mosi = gpio_out_setup(mosi_pin, 0);
    spi->miso = gpio_in_setup(miso_pin, 1);
    spi->mode = mode;
}
DECL_COMMAND(command_config_software_spi,
             "config_software_spi oid=%c sclk_pin=%u mosi_pin=%u miso_pin=%u"
             " mode=%u rate=%u");

struct spi_software *
spi_software_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_software_spi);
}

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
