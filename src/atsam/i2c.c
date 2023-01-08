// SAM4 I2C Port
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/misc.h" // timer_from_us
#include "command.h" // shutdown
#include "gpio.h" // i2c_setup
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown

#if CONFIG_MACH_SAME70
#include "same70_i2c.h" // Fixes for upstream header changes
#endif

struct twi_info {
    void *dev;
    uint32_t dev_id;
    uint8_t scl_pin, sda_pin, periph;
};

// I2C pin definitions
#if CONFIG_MACH_SAM3X
DECL_ENUMERATION_RANGE("i2c_bus", "twi0", 0, 2);
DECL_CONSTANT_STR("BUS_PINS_twi0", "PA18,PA17");
DECL_CONSTANT_STR("BUS_PINS_twi1", "PB13,PB12");
#define PRD_CALC_NUM 4
#elif CONFIG_MACH_SAM4
DECL_ENUMERATION_RANGE("i2c_bus", "twi0", 0, 2);
DECL_CONSTANT_STR("BUS_PINS_twi0", "PA4,PA3");
DECL_CONSTANT_STR("BUS_PINS_twi1", "PB5,PB4");
#define PRD_CALC_NUM 4
#elif CONFIG_MACH_SAME70
DECL_ENUMERATION_RANGE("i2c_bus", "twihs0", 0,3);
DECL_CONSTANT_STR("BUS_PINS_twihs0", "PA4,PA3");
DECL_CONSTANT_STR("BUS_PINS_twihs1", "PB5,PB4");
DECL_CONSTANT_STR("BUS_PINS_twihs2", "PD28,PD27");
#define PRD_CALC_NUM 3
#endif

static const struct twi_info twi_bus[] = {
#if CONFIG_MACH_SAM3X
    { TWI0, ID_TWI0, GPIO('A', 18), GPIO('A', 17), 'A'},
    { TWI1, ID_TWI1, GPIO('B', 13), GPIO('B', 12), 'A'},
#elif CONFIG_MACH_SAM4
    { TWI0, ID_TWI0, GPIO('A', 4), GPIO('A', 3), 'A'},
    { TWI1, ID_TWI1, GPIO('B', 5), GPIO('B', 4), 'A'},
#elif CONFIG_MACH_SAME70
    { TWIHS0, ID_TWIHS0, GPIO('A', 4), GPIO('A', 3), 'A'},
    { TWIHS1, ID_TWIHS1, GPIO('B', 5), GPIO('B', 4), 'A'},
    { TWIHS2, ID_TWIHS2, GPIO('D', 28), GPIO('D', 27), 'C'},
#endif
};

static void
init_pins(uint32_t bus)
{
    const struct twi_info *si = &twi_bus[bus];
    gpio_peripheral(si->scl_pin, si->periph, 1);
    gpio_peripheral(si->sda_pin, si->periph, 1);
    enable_pclock(si->dev_id);
}

static void
i2c_init(uint32_t bus, uint32_t rate)
{
    Twi *p_twi = twi_bus[bus].dev;
    p_twi->TWI_IDR = 0xFFFFFFFF;
    (void)p_twi->TWI_SR;
    p_twi->TWI_CR = TWI_CR_SWRST;
    (void)p_twi->TWI_RHR;
    p_twi->TWI_CR = TWI_CR_MSDIS;
    p_twi->TWI_CR = TWI_CR_SVDIS;
    p_twi->TWI_CR = TWI_CR_MSEN;

    uint32_t pclk = get_pclock_frequency(twi_bus[bus].dev_id);
    uint32_t cldiv = 0, chdiv = 0, ckdiv = 0;
    cldiv = pclk / ((rate > 384000 ? 384000 : rate) * 2) - PRD_CALC_NUM;

    while ((cldiv > 255) && (ckdiv < 7)) {
        ckdiv++;
        cldiv /= 2;
    }

    if (rate > 348000) {
        chdiv = pclk / ((2 * rate - 384000) * 2) - PRD_CALC_NUM;
        while ((chdiv > 255) && (ckdiv < 7)) {
            ckdiv++;
            chdiv /= 2;
        }
    } else {
        chdiv = cldiv;
    }
    p_twi->TWI_CWGR = (TWI_CWGR_CLDIV(cldiv) | TWI_CWGR_CHDIV(chdiv)
                       | TWI_CWGR_CKDIV(ckdiv));
}

static uint32_t
addr_to_u32(uint8_t addr_len, uint8_t *addr)
{
    uint32_t address = addr[0];
    if (addr_len > 1) {
        address <<= 8;
        address |= addr[1];
    }
    if (addr_len > 2) {
        address <<= 8;
        address |= addr[2];
    }
    if (addr_len > 3) {
        shutdown("Addresses larger than 3 bytes are not supported");
    }
    return address;
}

struct i2c_config
i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
    if (bus >= ARRAY_SIZE(twi_bus)  || rate > 400000)
        shutdown("Invalid i2c_setup parameters!");
    Twi *p_twi = twi_bus[bus].dev;
    init_pins(bus);
    i2c_init(bus, rate);
    return (struct i2c_config){ .twi=p_twi, .addr=addr};
}

void
i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
    Twi *p_twi = config.twi;
    uint32_t timeout = timer_read_time() + timer_from_us(5000);
    uint32_t status, bytes_to_send = write_len;
    p_twi->TWI_MMR = TWI_MMR_DADR(config.addr);
    for (;;) {
        status = p_twi->TWI_SR;
        if (status & TWI_SR_NACK)
            shutdown("I2C NACK error encountered!");
        if (!(status & TWI_SR_TXRDY)) {
            if (!timer_is_before(timer_read_time(), timeout))
                shutdown("I2C timeout occured");
            continue;
        }
        if (!bytes_to_send)
            break;
        p_twi->TWI_THR = *write++;
        bytes_to_send--;
    }
    p_twi->TWI_CR = TWI_CR_STOP;
    while (!(p_twi->TWI_SR & TWI_SR_TXCOMP))
        ;
}

void
i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
         , uint8_t read_len, uint8_t *read)
{
    Twi *p_twi = config.twi;
    uint32_t timeout = timer_read_time() + timer_from_us(5000);
    uint32_t status, bytes_to_send = read_len;
    uint8_t stop = 0;
    p_twi->TWI_MMR = 0;
    p_twi->TWI_MMR = (TWI_MMR_MREAD | TWI_MMR_DADR(config.addr)
                      | ((reg_len << TWI_MMR_IADRSZ_Pos) & TWI_MMR_IADRSZ_Msk));
    p_twi->TWI_IADR = 0;
    p_twi->TWI_IADR = addr_to_u32(reg_len, reg);
    if (bytes_to_send == 1) {
        p_twi->TWI_CR = TWI_CR_START | TWI_CR_STOP;
        stop = 1;
    } else {
        p_twi->TWI_CR = TWI_CR_START;
        stop = 0;
    }
    while (bytes_to_send > 0) {
        status = p_twi->TWI_SR;
        if (status & TWI_SR_NACK) {
            shutdown("I2C NACK error encountered!");
        }
        if (bytes_to_send == 1 && !stop) {
            p_twi->TWI_CR = TWI_CR_STOP;
            stop = 1;
        }
        if (!(status & TWI_SR_RXRDY)) {
            if (!timer_is_before(timer_read_time(), timeout))
                shutdown("I2C timeout occured");
            continue;
        }
        *read++ = p_twi->TWI_RHR;
        bytes_to_send--;
    }
    while (!(p_twi->TWI_SR & TWI_SR_TXCOMP))
        ;
    (void)p_twi->TWI_SR;
}
