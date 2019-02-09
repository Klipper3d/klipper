// Software SPI emulation
//
// Copyright (C) 2019  XXX
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/gpio.h" // gpio_out_setup
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND

struct spi_software {
    struct gpio_out sclk, mosi;
    struct gpio_in miso;
    uint8_t mode;
};


void
command_config_software_spi(uint32_t *args)
{
    uint8_t oid = args[0],
            sclk_pin = args[1],
            mosi_pin = args[2],
            miso_pin = args[3];

    struct spi_software *spi = oid_alloc(oid, command_config_software_spi
            , sizeof(*spi) + 1);

    spi->sclk = gpio_out_setup(sclk_pin, 1);
    spi->mosi = gpio_out_setup(mosi_pin, 1);
    spi->miso = gpio_in_setup(miso_pin, 1);
}
DECL_COMMAND(command_config_software_spi,
             "config_software_spi oid=%c sclk_pin=%u mosi_pin=%u miso_pin=%u");

struct spi_software *
spi_software_setup(uint8_t oid, uint8_t mode, uint32_t rate)
{
    struct spi_software *spi = oid_lookup(oid, command_config_software_spi);
    spi->mode = mode;
    gpio_out_write(spi->sclk, mode < 2 ? 0 : 1);

    return spi;
}

void
spi_software_prepare(struct spi_software *ss)
{
    gpio_out_write(ss->sclk, ss->mode < 2 ? 0 : 1);
}

void
spi_software_transfer(struct spi_software *ss, uint8_t receive_data
                      , uint8_t len, uint8_t *data)
{
    uint8_t outbuf = *data;
    uint8_t inbuf = 0;
    while (len--) {
        for (uint_fast8_t i = 0; i < 8; i++) {
            if (ss->mode % 2 == 0) {
                // MODE 0 & 2
                gpio_out_write(ss->mosi, outbuf | 0x80);
                outbuf <<= 1;
                gpio_out_toggle(ss->sclk); // mode 0 high - mode 2 low
                inbuf <<= 1;
                inbuf |= gpio_in_read(ss->miso);
                gpio_out_toggle(ss->sclk); // mode 0 low - mode 2 high
                break;
            } else {
                // MODE 1 & 3
                gpio_out_toggle(ss->sclk); // mode 1 high - mode 3 low
                gpio_out_write(ss->mosi, outbuf | 0x80);
                outbuf <<= 1;
                gpio_out_toggle(ss->sclk); // mode 1 low - mode 3 high
                inbuf <<= 1;
                inbuf |= gpio_in_read(ss->miso);
            }
        }
    }

    if (receive_data) *data = inbuf;
}
