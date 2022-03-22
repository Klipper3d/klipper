// SPI transmissions on sam3
//
// Copyright (C) 2018  Petri Honkala <cruwaller@gmail.com>
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // shutdown
#include "gpio.h" // spi_setup
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown

#if CONFIG_MACH_SAME70 // Fixes for upstream header changes
#define US_MR_CHMODE_NORMAL US_MR_USART_CHMODE_NORMAL
#define US_MR_CPHA US_MR_SPI_CPHA
#define US_MR_CPOL US_MR_SPI_CPOL
#endif

/****************************************************************
 * SPI/USART buses and pins
 ****************************************************************/

struct spi_info {
    void *dev;
    uint32_t dev_id;
    uint8_t miso_pin, mosi_pin, sck_pin, rx_periph, tx_periph, sck_periph;
};

#if CONFIG_MACH_SAM3X
DECL_ENUMERATION("spi_bus", "spi0", 0);
DECL_ENUMERATION_RANGE("spi_bus", "usart0", 1, 3);
DECL_CONSTANT_STR("BUS_PINS_spi0", "PA25,PA26,PA27");
DECL_CONSTANT_STR("BUS_PINS_usart0", "PA10,PA11,PA17");
DECL_CONSTANT_STR("BUS_PINS_usart1", "PA12,PA13,PA16");
DECL_CONSTANT_STR("BUS_PINS_usart2", "PB21,PB20,PB24");
#elif CONFIG_MACH_SAM4S
DECL_ENUMERATION("spi_bus", "spi", 0);
DECL_ENUMERATION_RANGE("spi_bus", "usart0", 1, 2);
DECL_CONSTANT_STR("BUS_PINS_spi", "PA12,PA13,PA14");
DECL_CONSTANT_STR("BUS_PINS_usart0", "PA5,PA6,PA2");
DECL_CONSTANT_STR("BUS_PINS_usart1", "PA21,PA22,PA23");
#elif CONFIG_MACH_SAM4E
DECL_ENUMERATION("spi_bus", "spi", 0);
DECL_ENUMERATION_RANGE("spi_bus", "usart0", 1, 2);
DECL_CONSTANT_STR("BUS_PINS_spi", "PA12,PA13,PA14");
DECL_CONSTANT_STR("BUS_PINS_usart0", "PB0,PB1,PB13");
DECL_CONSTANT_STR("BUS_PINS_usart1", "PA21,PA22,PA23");
#elif CONFIG_MACH_SAME70
DECL_ENUMERATION_RANGE("spi_bus", "spi0", 0, 2);
DECL_ENUMERATION_RANGE("spi_bus", "usart0", 2, 3);
DECL_CONSTANT_STR("BUS_PINS_spi0", "PD20,PD21,PD22");
DECL_CONSTANT_STR("BUS_PINS_usart0", "PB0,PB1,PB13");
DECL_CONSTANT_STR("BUS_PINS_usart1", "PA21,PB4,PA23");
DECL_CONSTANT_STR("BUS_PINS_usart2", "PD15,PD16,PD17");
#endif

static const struct spi_info spi_bus[] = {
#if CONFIG_MACH_SAM3X
    { SPI0, ID_SPI0,
        GPIO('A', 25), GPIO('A', 26), GPIO('A', 27), 'A', 'A', 'A'},
    { USART0, ID_USART0,
        GPIO('A', 10), GPIO('A', 11), GPIO('A', 17), 'A', 'A', 'B'},
    { USART1, ID_USART1,
        GPIO('A', 12), GPIO('A', 13), GPIO('A', 16), 'A', 'A', 'A'},
    { USART2, ID_USART2,
        GPIO('B', 21), GPIO('B', 20), GPIO('B', 24), 'A', 'A', 'A'},
#elif CONFIG_MACH_SAM4S
    { SPI, ID_SPI,
        GPIO('A', 12), GPIO('A', 13), GPIO('A', 14), 'A', 'A', 'A'},
    { USART0, ID_USART0,
        GPIO('A', 5),  GPIO('A', 6),  GPIO('A', 2),  'A', 'A', 'B'},
    { USART1, ID_USART1,
        GPIO('A', 21), GPIO('A', 22), GPIO('A', 23), 'A', 'A', 'A'},
#elif CONFIG_MACH_SAM4E
    { SPI, ID_SPI,
        GPIO('A', 12), GPIO('A', 13), GPIO('A', 14), 'A', 'A', 'A'},
    { USART0, ID_USART0,
        GPIO('B', 0),  GPIO('B', 1),  GPIO('B', 13), 'C', 'C', 'C'},
    { USART1, ID_USART1,
        GPIO('A', 21), GPIO('A', 22), GPIO('A', 23), 'A', 'A', 'A'},
#elif CONFIG_MACH_SAME70
    { SPI0, ID_SPI0,
        GPIO('D', 20), GPIO('D', 21), GPIO('D', 22), 'B', 'B', 'B'},
    { SPI1, ID_SPI1,
        GPIO('C', 26), GPIO('C', 27), GPIO('C', 24), 'C', 'C', 'C'},
    { USART0, ID_USART0,
        GPIO('B', 0),  GPIO('B', 1),  GPIO('B', 13), 'C', 'C', 'C'},
    { USART1, ID_USART1,
        GPIO('A', 21), GPIO('B', 4),  GPIO('A', 23), 'A', 'D', 'A'},
    { USART2, ID_USART2,
        GPIO('D', 15), GPIO('D', 16), GPIO('D', 17), 'B', 'B', 'B'},
#endif
};

static int
is_spihw(void *dev)
{
#if CONFIG_MACH_SAM3X
    return dev == SPI0;
#elif CONFIG_MACH_SAME70
    return (dev == SPI0) || (dev == SPI1);
#else
    return dev == SPI;
#endif
}

static void
init_pins(uint32_t bus)
{
    const struct spi_info *si = &spi_bus[bus];
    gpio_peripheral(si->sck_pin, si->sck_periph, 0);
    gpio_peripheral(si->miso_pin, si->rx_periph, 1);
    gpio_peripheral(si->mosi_pin, si->tx_periph, 0);
    enable_pclock(si->dev_id);
}


/****************************************************************
 * SPI hardware
 ****************************************************************/

#define CHANNEL    0 // Use same channel for all

static void
spihw_init(uint32_t bus)
{
    init_pins(bus);
    Spi *pSpi = spi_bus[bus].dev;

    /* Disable SPI */
    pSpi->SPI_CR  = SPI_CR_SPIDIS;
    /* Execute a software reset of the SPI twice */
    pSpi->SPI_CR  = SPI_CR_SWRST;
    pSpi->SPI_CR  = SPI_CR_SWRST;

    pSpi->SPI_MR = ( SPI_MR_MSTR |       // Set master mode
                     SPI_MR_MODFDIS |    // Disable fault detection
                     SPI_MR_PCS(CHANNEL) // Fixes peripheral select
                   );
    pSpi->SPI_IDR = 0xffffffff; // Disable ISRs

    /* Clear Chip Select Registers */
    pSpi->SPI_CSR[0] = 0;
    pSpi->SPI_CSR[1] = 0;
    pSpi->SPI_CSR[2] = 0;
    pSpi->SPI_CSR[3] = 0;

    /* Set basic channel config */
    pSpi->SPI_CSR[CHANNEL] = 0;
    /* Enable SPI */
    pSpi->SPI_CR = SPI_CR_SPIEN;
}

static struct spi_config
spihw_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    // Make sure bus is enabled
    spihw_init(bus);

    uint32_t clockDiv, pclk = get_pclock_frequency(ID_USART0);
    if (rate < pclk / 255) {
        clockDiv = 255;
    } else if (rate >= pclk / 2) {
        clockDiv = 2;
    } else {
        clockDiv = pclk / (rate + 1) + 1;
    }

    /****** Will be written to SPI_CSRx register ******/
    // CSAAT  : Chip Select Active After Transfer
    uint32_t config = SPI_CSR_CSAAT;
    config |= SPI_CSR_BITS_8_BIT; // TODO: support for SPI_CSR_BITS_16_BIT
    // NOTE: NCPHA is inverted, CPHA normal!!
    switch(mode) {
        case 0:
            config |= SPI_CSR_NCPHA;
            break;
        case 1:
            config |= 0;
            break;
        case 2:
            config |= SPI_CSR_NCPHA;
            config |= SPI_CSR_CPOL;
            break;
        case 3:
            config |= SPI_CSR_CPOL;
            break;
    };

    config |= ((clockDiv & 0xffu) << SPI_CSR_SCBR_Pos);
    return (struct spi_config){ .spidev = spi_bus[bus].dev, .cfg = config };
}

static void
spihw_prepare(struct spi_config config)
{
    Spi *pSpi = config.spidev;
    pSpi->SPI_CSR[CHANNEL] = config.cfg;
}

static void
spihw_transfer(struct spi_config config, uint8_t receive_data
               , uint8_t len, uint8_t *data)
{
    Spi *pSpi = config.spidev;
    if (receive_data) {
        while (len--) {
            pSpi->SPI_TDR = *data;
            // wait for receive register
            while (!(pSpi->SPI_SR & SPI_SR_RDRF))
                ;
            // get data
            *data++ = pSpi->SPI_RDR;
        }
    } else {
        while (len--) {
            pSpi->SPI_TDR = *data++;
            // wait for receive register
            while (!(pSpi->SPI_SR & SPI_SR_RDRF))
                ;
            // read data (to clear RDRF)
            pSpi->SPI_RDR;
        }
    }
}


/****************************************************************
 * USART hardware
 ****************************************************************/

static struct spi_config
usart_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    init_pins(bus);
    Usart *p_usart = spi_bus[bus].dev;

    p_usart->US_MR = 0;
    p_usart->US_RTOR = 0;
    p_usart->US_TTGR = 0;

    p_usart->US_CR = US_CR_RSTTX | US_CR_RSTRX | US_CR_TXDIS | US_CR_RXDIS;

    uint32_t pclk = get_pclock_frequency(ID_USART0);
    uint32_t br = DIV_ROUND_UP(pclk, rate);
    p_usart->US_BRGR = br << US_BRGR_CD_Pos;

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
    return (struct spi_config){ .spidev=p_usart, .cfg=p_usart->US_MR };
}

static void
usart_prepare(struct spi_config config)
{
}

static void
usart_transfer(struct spi_config config, uint8_t receive_data
               , uint8_t len, uint8_t *data)
{
    Usart *p_usart = config.spidev;
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


/****************************************************************
 * Interface
 ****************************************************************/

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    if (bus >= ARRAY_SIZE(spi_bus))
        shutdown("Invalid spi bus");
    if (is_spihw(spi_bus[bus].dev))
        return spihw_setup(bus, mode, rate);
    return usart_setup(bus, mode, rate);
}

void
spi_prepare(struct spi_config config)
{
    if (is_spihw(config.spidev))
        spihw_prepare(config);
    else
        usart_prepare(config);
}

void
spi_transfer(struct spi_config config, uint8_t receive_data
             , uint8_t len, uint8_t *data)
{
    if (is_spihw(config.spidev))
        spihw_transfer(config, receive_data, len, data);
    else
        usart_transfer(config, receive_data, len, data);
}
