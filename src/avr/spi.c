// Serial Peripheral Interface (SPI) support
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_MACH_atmega644p
#include "command.h" // shutdown
#include "gpio.h" // spi_setup
#include "internal.h" // GPIO
#include "pgm.h" // READP
#include "sched.h" // sched_shutdown

#if CONFIG_MACH_atmega168 || CONFIG_MACH_atmega328 || CONFIG_MACH_atmega328p
static const uint8_t SS = GPIO('B', 2), SCK = GPIO('B', 5);
static const uint8_t MOSI = GPIO('B', 3), MISO = GPIO('B', 4);
#elif CONFIG_MACH_atmega644p || CONFIG_MACH_atmega1284p
static const uint8_t SS = GPIO('B', 4), SCK = GPIO('B', 7);
static const uint8_t MOSI = GPIO('B', 5), MISO = GPIO('B', 6);
#elif CONFIG_MACH_at90usb1286 || CONFIG_MACH_at90usb646 || CONFIG_MACH_atmega32u4 || CONFIG_MACH_atmega1280 || CONFIG_MACH_atmega2560
static const uint8_t SS = GPIO('B', 0), SCK = GPIO('B', 1);
static const uint8_t MOSI = GPIO('B', 2), MISO = GPIO('B', 3);
#endif

static void
spi_init(void)
{
    if (!(GPIO2REGS(SS)->mode & GPIO2BIT(SS)))
        // The SS pin must be an output pin (but is otherwise unused)
        gpio_out_setup(SS, 0);
    gpio_out_setup(SCK, 0);
    gpio_out_setup(MOSI, 0);
    gpio_in_setup(MISO, 0);

    SPCR = (1<<MSTR) | (1<<SPE);
    SPSR = 0;
}

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    if (bus)
        shutdown("Invalid spi_setup parameters");

    // Make sure the SPI interface is enabled
    spi_init();

    // Setup rate
    struct spi_config config = {0, 0};
    if (rate >= (CONFIG_CLOCK_FREQ / 2)) {
        config.spsr = (1<<SPI2X);
    } else if (rate >= (CONFIG_CLOCK_FREQ / 4)) {
        config.spcr = 0;
    } else if (rate >= (CONFIG_CLOCK_FREQ / 8)) {
        config.spcr = 1;
        config.spsr = (1<<SPI2X);
    } else if (rate >= (CONFIG_CLOCK_FREQ / 16)) {
        config.spcr = 1;
    } else if (rate >= (CONFIG_CLOCK_FREQ / 32)) {
        config.spcr = 2;
        config.spsr = (1<<SPI2X);
    } else if (rate >= (CONFIG_CLOCK_FREQ / 64)) {
        config.spcr = 2;
    } else {
        config.spcr = 3;
    }

    // Setup mode
    config.spcr |= (1<<SPE) | (1<<MSTR);
    switch(mode) {
        case 0: {
            // MODE 0 - CPOL=0, CPHA=0
            break;
        }
        case 1: {
            // MODE 1 - CPOL=0, CPHA=1
            config.spcr |= (1<<CPHA);
            break;
        }
        case 2: {
            // MODE 2 - CPOL=1, CPHA=0
            config.spcr |= (1<<CPOL);
            break;
        }
        case 3: {
            // MODE 3 - CPOL=1, CPHA=1
            config.spcr |= (1<<CPOL) | (1<<CPHA);
            break;
        }
    }

    return config;
}

void
spi_prepare(struct spi_config config)
{
    SPCR = config.spcr;
    SPSR = config.spsr;
}

void
spi_transfer(struct spi_config config, uint8_t receive_data
             , uint8_t len, uint8_t *data)
{
    if (receive_data) {
        while (len--) {
            SPDR = *data;
            while (!(SPSR & (1<<SPIF)))
                ;
            *data++ = SPDR;
        }
    } else {
        while (len--) {
            SPDR = *data++;
            while (!(SPSR & (1<<SPIF)))
                ;
        }
    }
}
