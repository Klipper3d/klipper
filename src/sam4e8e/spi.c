// SAM4e8e SPI port
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// Klipper
#include "command.h" // shutdown
#include "sched.h"
#include "autoconf.h" // CONFIG_CLOCK_FREQ

// SAM4E port
#include "sam.h"
#include "gpio.h"

#define SSPI_USART0 0
#define SSPI_USART1 1
#define SSPI_SPI    2

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    Usart *p_usart = USART0;
    if (bus > 2) {
        shutdown("Invalid spi_setup parameters");
    }

    if (bus == SSPI_USART0) {
        // DUET_USART0_SCK as per dc42 CoreNG
        gpio_set_peripheral('B', PIO_PB13C_SCK0, 'C', 0);
        // DUET_USART0_MOSI as per dc42 CoreNG
        gpio_set_peripheral('B', PIO_PB1C_TXD0, 'C', 0);
        // DUET_USART0_MISO as per dc42 CoreNG
        gpio_set_peripheral('B', PIO_PB0C_RXD0, 'C', 1);

        if ((PMC->PMC_PCSR0 & (1u << ID_USART0)) == 0) {
            PMC->PMC_PCER0 = 1 << ID_USART0;
        }
        p_usart = USART0;
    } else if (bus == SSPI_USART1) {
        // DUET_USART1_SCK as per dc42 CoreNG
        gpio_set_peripheral('A', PIO_PA23A_SCK1, 'A', 0);
        // DUET_USART1_MOSI as per dc42 CoreNG
        gpio_set_peripheral('A', PIO_PA22A_TXD1, 'A', 0);
        // DUET_USART1_MISO as per dc42 CoreNG
        gpio_set_peripheral('A', PIO_PA21A_RXD1, 'A', 1);

        if ((PMC->PMC_PCSR0 & (1u << ID_USART1)) == 0) {
            PMC->PMC_PCER0 = 1 << ID_USART1;
        }
        p_usart = USART1;
    }

    if (bus < 2) {
        p_usart->US_MR = 0;
        p_usart->US_RTOR = 0;
        p_usart->US_TTGR = 0;

        p_usart->US_CR = US_CR_RSTTX | US_CR_RSTRX | US_CR_TXDIS | US_CR_RXDIS;

        uint32_t br = (CONFIG_CLOCK_FREQ + rate / 2) / rate;
        p_usart-> US_BRGR = br << US_BRGR_CD_Pos;

        uint32_t reg = US_MR_CHRL_8_BIT |
                       US_MR_USART_MODE_SPI_MASTER |
                       US_MR_CLKO |
                       US_MR_CHMODE_NORMAL;
        switch (mode) {
        case 0:
            reg |= US_MR_CPHA;
            reg &= ~US_MR_CPOL;
            break;
        case 1:
            reg &= ~US_MR_CPHA;
            reg &= ~US_MR_CPOL;
            break;
        case 2:
            reg |= US_MR_CPHA;
            reg |= US_MR_CPOL;
            break;
        case 3:
            reg &= ~US_MR_CPHA;
            reg |= US_MR_CPOL;
            break;
        }

        p_usart->US_MR |= reg;
        p_usart->US_CR = US_CR_RXEN | US_CR_TXEN;
        return (struct spi_config){ .sspi=p_usart, .cfg=p_usart->US_MR };
    }

    // True SPI implementation still ToDo
    return (struct spi_config){ .sspi = 0, .cfg=0};
}

void
spi_transfer(struct spi_config config, uint8_t receive_data
             , uint8_t len, uint8_t *data)
{
    if ((config.sspi == USART0) || (config.sspi == USART1)) {
        Usart *p_usart = config.sspi;
        if (receive_data) {
            for (uint32_t i = 0; i < len; ++i) {
                uint32_t co = (uint32_t)*data & 0x000000FF;
                while(!(p_usart->US_CSR & US_CSR_TXRDY)) {}
                p_usart->US_THR = US_THR_TXCHR(co);
                uint32_t ci = 0;
                while(!(p_usart->US_CSR & US_CSR_RXRDY)) {}
                ci = p_usart->US_RHR & US_RHR_RXCHR_Msk;
                *data++ = (uint8_t)ci;
            }
        } else {
            for (uint32_t i = 0; i < len; ++i) {
                uint32_t co = (uint32_t)*data & 0x000000FF;
                while(!(p_usart->US_CSR & US_CSR_TXRDY)) {}
                p_usart->US_THR = US_THR_TXCHR(co);
                while(!(p_usart->US_CSR & US_CSR_RXRDY)) {}
                (void)(p_usart->US_RHR & US_RHR_RXCHR_Msk);
                (void)*data++;
            }
        }
    }
}

void
spi_prepare(struct spi_config config) {}
