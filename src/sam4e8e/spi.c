// SPI transmissions on sam4e8e
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// Klipper
#include "command.h" // shutdown
#include "sched.h"

// SAM4E port
#include "gpio.h"

// ASF
#include "pmc/pmc.h" // pmc_enable_periph_clk
#include "pio/pio.h" // pio_configure
#include "usart/usart.h" // usart*
#include "sysclk.h"

#define SSPI_USART0 0
#define SSPI_USART1 1
#define SSPI_SPI    2

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    Usart *p_usart = USART0;
    if (bus > 2 || mode  > 3) {
        shutdown("Invalid spi_setup parameters");
    }

    if (bus == SSPI_USART0) {
        // DUET_USART0_SCK as per dc42 CoreNG
        pio_configure(PIOB, PIO_PERIPH_C, PIO_PB13C_SCK0, PIO_DEFAULT);
        // DUET_USART0_MOSI as per dc42 CoreNG
        pio_configure(PIOB, PIO_PERIPH_C, PIO_PB1C_TXD0, PIO_DEFAULT);
        // DUET_USART0_MISO as per dc42 CoreNG
        pio_configure(PIOB, PIO_PERIPH_C, PIO_PB0C_RXD0, PIO_PULLUP);

        sysclk_enable_peripheral_clock(ID_USART0);
        p_usart = USART0;
    } else if (bus == SSPI_USART1) {
        // DUET_USART1_SCK as per dc42 CoreNG
        pio_configure(PIOA, PIO_PERIPH_A, PIO_PA23A_SCK1, PIO_DEFAULT);
        // DUET_USART1_MOSI as per dc42 CoreNG
        pio_configure(PIOA, PIO_PERIPH_A, PIO_PA22A_TXD1, PIO_DEFAULT);
        // DUET_USART1_MISO as per dc42 CoreNG
        pio_configure(PIOA, PIO_PERIPH_A, PIO_PA21A_RXD1, PIO_PULLUP);

        sysclk_enable_peripheral_clock(ID_USART1);
        p_usart = USART1;
    }

    usart_spi_opt_t opts = {.baudrate=rate, \
                            .char_length=US_MR_CHRL_8_BIT, \
                            .spi_mode=(uint8_t)mode, \
                            .channel_mode=US_MR_CHMODE_NORMAL};
    usart_init_spi_master(p_usart, &opts, sysclk_get_peripheral_hz());

    usart_enable_rx(p_usart);
    usart_enable_tx(p_usart);

    return (struct spi_config){ .sspi=p_usart, .cfg=p_usart->US_MR };
}

void
spi_transfer(struct spi_config config, uint8_t receive_data
             , uint8_t len, uint8_t *data)
{
    if (receive_data) {
        for (uint32_t i = 0; i < len; ++i) {
            uint32_t co = (uint32_t)*data & 0x000000FF;
            usart_putchar(config.sspi, co);
            uint32_t ci = 0;
            usart_getchar(config.sspi, &ci);
            *data++ = (uint8_t)ci;
        }
    } else {
        for (uint32_t i = 0; i < len; ++i) {
            uint32_t co = (uint32_t)*data & 0x000000FF;
            usart_putchar(config.sspi, co & 0xFF);
            uint32_t ci = 0;
            usart_getchar(config.sspi, &ci);
            (void)*data++;
        }
    }
}

void
spi_prepare(struct spi_config config) {}
