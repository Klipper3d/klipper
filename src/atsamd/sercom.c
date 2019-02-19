// Handling of sercom pins
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "internal.h" // sercom_enable
#include "command.h" // shutdown
#include "compiler.h" // ARRAY_SIZE
#include "sched.h" // sched_shutdown


/****************************************************************
 * Available sercom blocks
 ****************************************************************/

struct sercom_bus {
    Sercom *sercom;
    uint32_t pclk_id, pm_id;
};

static const struct sercom_bus sercoms[6] = {
    { SERCOM0, SERCOM0_GCLK_ID_CORE, ID_SERCOM0 },
    { SERCOM1, SERCOM1_GCLK_ID_CORE, ID_SERCOM1 },
    { SERCOM2, SERCOM2_GCLK_ID_CORE, ID_SERCOM2 },
    { SERCOM3, SERCOM3_GCLK_ID_CORE, ID_SERCOM3 },
    { SERCOM4, SERCOM4_GCLK_ID_CORE, ID_SERCOM4 },
    { SERCOM5, SERCOM5_GCLK_ID_CORE, ID_SERCOM5 },
};

Sercom *
sercom_enable(uint32_t sercom_id)
{
    if (sercom_id >= ARRAY_SIZE(sercoms))
        shutdown("Invalid sercom bus");
    const struct sercom_bus *sb = &sercoms[sercom_id];
    enable_pclock(sb->pclk_id, sb->pm_id);
    return sb->sercom;
}


/****************************************************************
 * Pin configurations
 ****************************************************************/

struct sercom_pad {
    uint8_t sercom_id, gpio, pad, ptype;
};

static const struct sercom_pad sercom_pads[] = {
    { 0, GPIO('A', 8), 0, 'C' },
    { 0, GPIO('A', 9), 1, 'C' },
    { 0, GPIO('A', 10), 2, 'C' },
    { 0, GPIO('A', 11), 3, 'C' },
    { 0, GPIO('A', 4), 0, 'D' },
    { 0, GPIO('A', 1), 1, 'D' },
    { 0, GPIO('A', 6), 2, 'D' },
    { 0, GPIO('A', 7), 3, 'D' },
    { 1, GPIO('A', 16), 0, 'C' },
    { 1, GPIO('A', 17), 1, 'C' },
    { 1, GPIO('A', 18), 2, 'C' },
    { 1, GPIO('A', 19), 3, 'C' },
    { 1, GPIO('A', 0), 0, 'D' },
    { 1, GPIO('A', 1), 1, 'D' },
    { 1, GPIO('A', 30), 2, 'D' },
    { 1, GPIO('A', 31), 3, 'D' },
    { 2, GPIO('A', 12), 0, 'C' },
    { 2, GPIO('A', 13), 1, 'C' },
    { 2, GPIO('A', 14), 2, 'C' },
    { 2, GPIO('A', 15), 3, 'C' },
    { 2, GPIO('A', 8), 0, 'D' },
    { 2, GPIO('A', 9), 1, 'D' },
    { 2, GPIO('A', 10), 2, 'D' },
    { 2, GPIO('A', 11), 3, 'D' },
    { 3, GPIO('A', 22), 0, 'C' },
    { 3, GPIO('A', 23), 1, 'C' },
    { 3, GPIO('A', 24), 2, 'C' },
    { 3, GPIO('A', 25), 3, 'C' },
    { 3, GPIO('A', 16), 0, 'D' },
    { 3, GPIO('A', 17), 1, 'D' },
    { 3, GPIO('A', 18), 2, 'D' },
    { 3, GPIO('A', 19), 3, 'D' },
    { 3, GPIO('A', 20), 2, 'D' },
    { 3, GPIO('A', 21), 3, 'D' },
    { 4, GPIO('B', 12), 0, 'C' },
    { 4, GPIO('B', 13), 1, 'C' },
    { 4, GPIO('B', 14), 2, 'C' },
    { 4, GPIO('B', 15), 3, 'C' },
    { 4, GPIO('B', 8), 0, 'D' },
    { 4, GPIO('B', 9), 1, 'D' },
    { 4, GPIO('B', 10), 2, 'D' },
    { 4, GPIO('B', 11), 3, 'D' },
    { 4, GPIO('A', 12), 0, 'D' },
    { 4, GPIO('A', 13), 1, 'D' },
    { 4, GPIO('A', 14), 2, 'D' },
    { 4, GPIO('A', 15), 3, 'D' },
    { 5, GPIO('B', 16), 0, 'C' },
    { 5, GPIO('B', 17), 1, 'C' },
    { 5, GPIO('A', 20), 2, 'C' },
    { 5, GPIO('A', 21), 3, 'C' },
    { 5, GPIO('A', 22), 0, 'D' },
    { 5, GPIO('A', 23), 1, 'D' },
    { 5, GPIO('A', 24), 2, 'D' },
    { 5, GPIO('A', 25), 3, 'D' },
    { 5, GPIO('B', 30), 0, 'D' },
    { 5, GPIO('B', 31), 1, 'D' },
    { 5, GPIO('B', 22), 2, 'D' },
    { 5, GPIO('B', 23), 3, 'D' },
    { 5, GPIO('B', 2), 0, 'D' },
    { 5, GPIO('B', 3), 1, 'D' },
    { 5, GPIO('B', 0), 2, 'D' },
    { 5, GPIO('B', 1), 3, 'D' },
};

static const struct sercom_pad *
sercom_lookup_pad(uint32_t sercom_id, uint8_t pin)
{
    const struct sercom_pad *sp = sercom_pads;
    for (; ; sp++) {
        if (sp >= &sercom_pads[ARRAY_SIZE(sercom_pads)])
            shutdown("Not a valid sercom pin");
        if (sp->sercom_id == sercom_id && sp->gpio == pin)
            return sp;
    }
}


/****************************************************************
 * Runtime configuration
 ****************************************************************/

enum { TX_PIN, RX_PIN, CLK_PIN };

// Runtime configuration
struct sercom_pin {
    uint8_t pins[3];
};

static struct sercom_pin sercom_pins[ARRAY_SIZE(sercoms)];

void
command_set_sercom_pin(uint32_t *args)
{
    uint8_t sercom_id = args[0], pin_type = args[1], pin = args[2];
    if (sercom_id >= ARRAY_SIZE(sercom_pins)
        || pin_type >= ARRAY_SIZE(sercom_pins[0].pins))
        shutdown("Invalid sercom bus");
    sercom_pins[sercom_id].pins[pin_type] = pin;
}
DECL_COMMAND(command_set_sercom_pin,
             "set_sercom_pin sercom_id=%u pin_type=%u pin=%u");


/****************************************************************
 * SPI dopo flag mapping
 ****************************************************************/

struct sercom_spi_map {
    uint8_t tx_pad, clk_pad, dopo;
};

static const struct sercom_spi_map sercom_spi[] = {
    { 0, 1, 0 },
    { 2, 3, 1 },
    { 3, 1, 2 },
    { 0, 3, 3 },
};

static uint8_t
sercom_lookup_spi_dopo(uint8_t tx_pad, uint8_t clk_pad)
{
    const struct sercom_spi_map *sm = sercom_spi;
    for (; ; sm++) {
        if (sm >= &sercom_spi[ARRAY_SIZE(sercom_spi)])
            shutdown("Not a valid sercom pin");
        if (sm->tx_pad == tx_pad && sm->clk_pad == clk_pad)
            return sm->dopo;
    }
}


/****************************************************************
 * Pin setup
 ****************************************************************/

uint32_t
sercom_spi_pins(uint32_t sercom_id)
{
    uint8_t tx_pin = sercom_pins[sercom_id].pins[TX_PIN];
    const struct sercom_pad *tx_sp = sercom_lookup_pad(sercom_id, tx_pin);
    uint8_t rx_pin = sercom_pins[sercom_id].pins[RX_PIN];
    const struct sercom_pad *rx_sp = sercom_lookup_pad(sercom_id, rx_pin);
    uint8_t clk_pin = sercom_pins[sercom_id].pins[CLK_PIN];
    const struct sercom_pad *clk_sp = sercom_lookup_pad(sercom_id, clk_pin);

    uint8_t dopo = sercom_lookup_spi_dopo(tx_sp->pad, clk_sp->pad);
    gpio_peripheral(tx_pin, tx_sp->ptype, 0);
    gpio_peripheral(rx_pin, rx_sp->ptype, 0);
    gpio_peripheral(clk_pin, clk_sp->ptype, 0);
    return SERCOM_SPI_CTRLA_DIPO(rx_sp->pad) | SERCOM_SPI_CTRLA_DOPO(dopo);
}

void
sercom_i2c_pins(uint32_t sercom_id)
{
    uint8_t tx_pin = sercom_pins[sercom_id].pins[TX_PIN];
    const struct sercom_pad *tx_sp = sercom_lookup_pad(sercom_id, tx_pin);
    uint8_t clk_pin = sercom_pins[sercom_id].pins[CLK_PIN];
    const struct sercom_pad *clk_sp = sercom_lookup_pad(sercom_id, clk_pin);

    if (tx_sp->pad != 0 || clk_sp->pad != 1)
        shutdown("Not a valid sercom pin");

    gpio_peripheral(tx_pin, tx_sp->ptype, 0);
    gpio_peripheral(clk_pin, clk_sp->ptype, 0);
}
