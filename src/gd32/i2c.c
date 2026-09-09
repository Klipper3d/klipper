// Hardware I2C support for GD32F30x and GD32E23x
//
// Copyright (C) 2026  Xiaoyue Cui <2508041672@qq.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"
#include "board/irq.h"
#include "board/misc.h"
#include "command.h"
#include "gpio.h"
#include "i2ccmds.h"
#include "internal.h"
#include "sched.h"

struct i2c_info {
    uint32_t i2c, clock;
    uint8_t scl_pin, sda_pin, alternate;
};

DECL_ENUMERATION("i2c_bus", "i2c0", 0);
DECL_CONSTANT_STR("BUS_PINS_i2c0", "PB6,PB7");

#if CONFIG_MACH_GD32F30X
DECL_ENUMERATION("i2c_bus", "i2c0a", 1);
DECL_CONSTANT_STR("BUS_PINS_i2c0a", "PB8,PB9");
DECL_ENUMERATION("i2c_bus", "i2c1", 2);
DECL_CONSTANT_STR("BUS_PINS_i2c1", "PB10,PB11");
static const struct i2c_info i2c_bus[] = {
    { I2C0, RCU_I2C0, GPIO('B', 6), GPIO('B', 7), 0 },
    { I2C0, RCU_I2C0, GPIO('B', 8), GPIO('B', 9), 1 },
    { I2C1, RCU_I2C1, GPIO('B', 10), GPIO('B', 11), 0 },
};
#else
// GD32E230 PB6/PB7 use AF1 for I2C0.
static const struct i2c_info i2c_bus[] = {
    { I2C0, RCU_I2C0, GPIO('B', 6), GPIO('B', 7), 1 },
};
#endif

static const struct i2c_info *active_bus[2];
static uint32_t active_rate[2];

static void
i2c_gpio_setup(const struct i2c_info *ii)
{
#if CONFIG_MACH_GD32F30X
    enable_pclock(RCU_AF);
    if (ii->i2c == I2C0)
        gpio_pin_remap_config(GPIO_I2C0_REMAP,
                              ii->alternate ? ENABLE : DISABLE);
    gpio_peripheral(ii->scl_pin, 3, 0);
    gpio_peripheral(ii->sda_pin, 3, 0);
#else
    gpio_init_af_set(ii->scl_pin, ii->alternate);
    gpio_init_af_set(ii->sda_pin, ii->alternate);
    gpio_init_mode_set(ii->scl_pin, GPIO_MODE_AF, GPIO_PUPD_PULLUP);
    gpio_init_mode_set(ii->sda_pin, GPIO_MODE_AF, GPIO_PUPD_PULLUP);
    gpio_init_output_options_set(ii->scl_pin, GPIO_OTYPE_OD);
    gpio_init_output_options_set(ii->sda_pin, GPIO_OTYPE_OD);
#endif
}

static const struct i2c_info *
i2c_get_info(uint32_t i2c)
{
    uint_fast8_t index = i2c == I2C0 ? 0 : 1;
    if (active_bus[index])
        return active_bus[index];
    uint_fast8_t i;
    for (i = 0; i < ARRAY_SIZE(i2c_bus); i++)
        if (i2c_bus[i].i2c == i2c)
            return &i2c_bus[i];
    shutdown("Unknown I2C peripheral");
}

static void
i2c_delay(void)
{
    uint32_t end = timer_read_time() + timer_from_us(5);
    while (timer_is_before(timer_read_time(), end))
        ;
}

// Recover a slave holding SDA low, then restore the peripheral registers.
static void
i2c_recover(uint32_t i2c)
{
    const struct i2c_info *ii = i2c_get_info(i2c);
    uint32_t ctl1 = I2C_CTL1(i2c), ckcfg = I2C_CKCFG(i2c), rt = I2C_RT(i2c);
    uint32_t scl = GPIO2BIT(ii->scl_pin), sda = GPIO2BIT(ii->sda_pin);
    I2C_CTL0(i2c) = 0;
#if CONFIG_MACH_GD32F30X
    gpio_peripheral(ii->scl_pin, 0, 0);
    gpio_peripheral(ii->sda_pin, 0, 0);
#else
    gpio_init_mode_set(ii->scl_pin, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP);
    gpio_init_mode_set(ii->sda_pin, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP);
    gpio_init_output_options_set(ii->scl_pin, GPIO_OTYPE_OD);
    gpio_init_output_options_set(ii->sda_pin, GPIO_OTYPE_OD);
#endif
    GPIO_BOP(GPIOB) = scl | sda;
    i2c_delay();
    uint_fast8_t pulses;
    for (pulses = 0; pulses < 9 && !(GPIO_ISTAT(GPIOB) & sda); pulses++) {
        GPIO_BC(GPIOB) = scl;
        i2c_delay();
        GPIO_BOP(GPIOB) = scl;
        i2c_delay();
    }
    // Generate a STOP while SCL is high.
    GPIO_BC(GPIOB) = sda;
    i2c_delay();
    GPIO_BOP(GPIOB) = scl;
    i2c_delay();
    GPIO_BOP(GPIOB) = sda;
    i2c_delay();
    i2c_gpio_setup(ii);
    I2C_CTL0(i2c) = I2C_CTL0_SRESET;
    I2C_CTL0(i2c) = 0;
    I2C_CTL1(i2c) = ctl1;
    I2C_CKCFG(i2c) = ckcfg;
    I2C_RT(i2c) = rt;
    I2C_STAT0(i2c) = 0;
    I2C_CTL0(i2c) = I2C_CTL0_I2CEN;
}

struct i2c_config
i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
    if (bus >= ARRAY_SIZE(i2c_bus) || !rate || rate > 400000 || addr > 0x7f)
        shutdown("Invalid I2C setup");
    const struct i2c_info *ii = &i2c_bus[bus];
    uint_fast8_t index = ii->i2c == I2C0 ? 0 : 1;
    if (active_bus[index] && active_bus[index] != ii)
        shutdown("Conflicting I2C pin mapping");
    if (active_rate[index] && active_rate[index] != rate)
        shutdown("Conflicting I2C bus rate");
    active_bus[index] = ii;
    active_rate[index] = rate;
    if (!is_enable_pclock(ii->clock)) {
        enable_pclock(ii->clock);
        i2c_gpio_setup(ii);
        I2C_CTL0(ii->i2c) = I2C_CTL0_SRESET;
        I2C_CTL0(ii->i2c) = 0;
        uint32_t pclk = APB1_FREQ;
        I2C_CTL1(ii->i2c) = pclk / 1000000;
        if (rate <= 100000) {
            uint32_t ccr = pclk / rate / 2;
            if (ccr < 4)
                ccr = 4;
            I2C_CKCFG(ii->i2c) = ccr;
            I2C_RT(ii->i2c) = pclk / 1000000 + 1;
        } else {
            uint32_t ccr = pclk / rate / 3;
            if (!ccr)
                ccr = 1;
            I2C_CKCFG(ii->i2c) = I2C_CKCFG_FAST | ccr;
            I2C_RT(ii->i2c) = pclk / 3000000 + 1;
        }
        I2C_CTL0(ii->i2c) = I2C_CTL0_I2CEN;
    }
    return (struct i2c_config){ .i2c=(void *)ii->i2c, .addr=addr << 1 };
}

static int
i2c_wait(uint32_t i2c, uint32_t set, uint32_t clear, uint32_t timeout)
{
    for (;;) {
        uint32_t stat = I2C_STAT0(i2c);
        if ((stat & set) == set && !(stat & clear))
            return I2C_BUS_SUCCESS;
        if (stat & I2C_STAT0_AERR) {
            I2C_STAT0(i2c) &= ~I2C_STAT0_AERR;
            return I2C_BUS_NACK;
        }
        if (stat & (I2C_STAT0_BERR | I2C_STAT0_LOSTARB | I2C_STAT0_OUERR)) {
            I2C_STAT0(i2c) &= ~(I2C_STAT0_BERR | I2C_STAT0_LOSTARB
                                | I2C_STAT0_OUERR);
            return I2C_BUS_TIMEOUT;
        }
        if (!timer_is_before(timer_read_time(), timeout))
            return I2C_BUS_TIMEOUT;
    }
}

static int
i2c_start(uint32_t i2c, uint8_t addr, uint8_t read_len, uint32_t timeout)
{
    if (I2C_STAT1(i2c) & I2C_STAT1_I2CBSY)
        i2c_recover(i2c);
    I2C_CTL0(i2c) = I2C_CTL0_START | I2C_CTL0_I2CEN;
    int ret = i2c_wait(i2c, I2C_STAT0_SBSEND, 0, timeout);
    if (ret)
        return ret;
    I2C_DATA(i2c) = addr;
    if (addr & 1)
        I2C_CTL0(i2c) |= I2C_CTL0_ACKEN;
    ret = i2c_wait(i2c, I2C_STAT0_ADDSEND, 0, timeout);
    irqstatus_t flag = irq_save();
    uint32_t stat1 = I2C_STAT1(i2c);
    if ((addr & 1) && read_len == 1)
        I2C_CTL0(i2c) = I2C_CTL0_STOP | I2C_CTL0_I2CEN;
    irq_restore(flag);
    if (!(stat1 & I2C_STAT1_MASTER) && ret == I2C_BUS_SUCCESS)
        return I2C_BUS_TIMEOUT;
    return ret;
}

static int
i2c_send_byte(uint32_t i2c, uint8_t data, uint32_t timeout)
{
    I2C_DATA(i2c) = data;
    return i2c_wait(i2c, I2C_STAT0_TBE, 0, timeout);
}

static int
i2c_stop(uint32_t i2c, uint32_t timeout)
{
    I2C_CTL0(i2c) = I2C_CTL0_STOP | I2C_CTL0_I2CEN;
    while (I2C_STAT1(i2c) & I2C_STAT1_I2CBSY) {
        if (!timer_is_before(timer_read_time(), timeout)) {
            i2c_recover(i2c);
            return I2C_BUS_TIMEOUT;
        }
    }
    return I2C_BUS_SUCCESS;
}

int
i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
    uint32_t i2c = (uint32_t)config.i2c;
    uint32_t timeout = timer_read_time() + timer_from_us(5000);
    int ret = i2c_start(i2c, config.addr, 0, timeout);
    if (ret == I2C_BUS_NACK)
        ret = I2C_BUS_START_NACK;
    while (write_len-- && ret == I2C_BUS_SUCCESS)
        ret = i2c_send_byte(i2c, *write++, timeout);
    int stop_ret = i2c_stop(i2c, timeout);
    return ret == I2C_BUS_SUCCESS ? stop_ret : ret;
}

int
i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg,
         uint8_t read_len, uint8_t *read)
{
    uint32_t i2c = (uint32_t)config.i2c;
    uint32_t timeout = timer_read_time() + timer_from_us(5000);
    int ret = I2C_BUS_SUCCESS;
    if (reg_len) {
        ret = i2c_start(i2c, config.addr, 0, timeout);
        if (ret == I2C_BUS_NACK)
            ret = I2C_BUS_START_NACK;
        while (reg_len-- && ret == I2C_BUS_SUCCESS)
            ret = i2c_send_byte(i2c, *reg++, timeout);
        if (ret != I2C_BUS_SUCCESS)
            goto abort;
    }
    if (!read_len)
        return i2c_stop(i2c, timeout);
    ret = i2c_start(i2c, config.addr | 1, read_len, timeout);
    if (ret == I2C_BUS_NACK)
        ret = I2C_BUS_START_READ_NACK;
    if (ret != I2C_BUS_SUCCESS)
        goto abort;
    while (read_len--) {
        ret = i2c_wait(i2c, I2C_STAT0_RBNE, 0, timeout);
        if (ret != I2C_BUS_SUCCESS)
            goto abort;
        irqstatus_t flag = irq_save();
        *read++ = I2C_DATA(i2c);
        if (read_len == 1)
            I2C_CTL0(i2c) &= ~I2C_CTL0_ACKEN;
        if (!read_len)
            I2C_CTL0(i2c) = I2C_CTL0_STOP | I2C_CTL0_I2CEN;
        irq_restore(flag);
    }
    return I2C_BUS_SUCCESS;
abort:
    i2c_stop(i2c, timeout);
    I2C_CTL0(i2c) &= ~I2C_CTL0_ACKEN;
    if (ret == I2C_BUS_TIMEOUT)
        i2c_recover(i2c);
    return ret;
}
