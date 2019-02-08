// Commands for sending messages on an SPI bus
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "board/gpio.h" // gpio_out_write
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_SHUTDOWN
#include "software_spi.h"

void software_spi_setup(struct software_spi_config spi_config) {
    gpio_out_setup(spi_config->pins->mosi, PIN_HIGH);
    gpio_in_setup(spi_config->pins->miso, PIN_HIGH);

    if (spi_config->pins->slave_select > 0) {
        gpio_out_setup(spi_config->pins->slave_select, PIN_HIGH);
    }

    if (spi_device->mode < 2) {
        gpio_in_setup(spi_config->pins->sysclock, PIN_LOW);
    } else {
        gpio_in_setup(spi_config->pins->sysclock, PIN_HIGH);
    }
}

uint8_t spi_read_bit(struct software_spi_config spi_device) {
    return gpio_in_read(spi_device->pins->miso) ? 1 : 0;
}

void spi_write_bit(struct software_spi_config spi_config, uint8_t data) {
    gpio_out_write(spi_config->pins->mosi, data);
}

void spi_clock_high(struct software_spi_config spi_config) {
    gpio_out_write(spi_config->pins->sysclock, PIN_HIGH);
}

void spi_clock_low(struct software_spi_config spi_config) {
    gpio_out_write(spi_config->pins->sysclock, PIN_LOW);
}

void spi_clock_toggle(struct software_spi_config spi_config) {
    gpio_out_toggle(spi_config->pins->sysclock);
}

void
software_spi_transfer(struct software_spi_config *config, uint8_t receive_data
        , uint8_t len, uint8_t *data)
{
    software_spi_setup(*config);


    while (len--) {
        uint8_t outbuf = *data;
        uint8_t inbuf = 0;
        for (uint_fast8_t i = 0; i < 8; i++) {

            if (config->mode % 2 == 0) {
                // MODE 0 & 2
                spi_write_bit(config->pins->mosi, outbuf | 0x80);
                outbuf <<= 1;
                spi_clock_toggle(); // mode 0 high - mode 2 low
                inbuf <<= 1;
                inbuf |= spi_read_bit(config->pins->miso);
                spi_clock_toggle(); // mode 0 low - mode 2 high
                break;
            } else {
                // MODE 1 & 3
                spi_clock_toggle(); // mode 1 high - mode 3 low
                spi_write_bit(config->pins->mosi, outbuf | 0x80);
                outbuf <<= 1;
                spi_clock_toggle(); // mode 1 low - mode 3 high
                inbuf <<= 1;
                inbuf |= spi_read_bit(config->pins->miso);
            }
        }

        if (receive_data) *data = inbuf;
        *data++;
    }
}

void
software_spi_shutdown(struct software_spi_config spi_config)
{
    software_spi_transfer(spi_config, 0, spi_config->shutdown_msg_len, spi_config->shutdown_msg);

    gpio_in_setup(spi_config->pins->mosi, PIN_LOW);
    gpio_in_setup(spi_config->pins->miso, PIN_LOW);
    gpio_in_setup(spi_config->pins->sysclock, PIN_LOW);

    if (spi_config->pins->slave_select > 0) {
        gpio_in_setup(spi_config->pins->slave_select, PIN_LOW);
    }
}
