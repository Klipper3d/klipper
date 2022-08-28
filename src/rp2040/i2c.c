// I2C functions for rp2040
//
// Copyright (C) 2022  Lasse Dalegaard <dalegaard@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/misc.h" // timer_is_before
#include "gpio.h" // i2c_setup, i2c_read, i2c_write
#include "command.h" // shutdown
#include "sched.h" // sched_shutdown
#include "internal.h" // pclock, gpio_peripheral
#include "hardware/regs/resets.h" // RESETS_RESET_I2C*_BITS
#include "hardware/structs/i2c.h"

struct i2c_info {
    i2c_hw_t *i2c;
    uint8_t sda_pin, scl_pin, pclk;
};

DECL_ENUMERATION("i2c_bus", "i2c0a", 0);
DECL_ENUMERATION("i2c_bus", "i2c0b", 1);
DECL_ENUMERATION("i2c_bus", "i2c0c", 2);
DECL_ENUMERATION("i2c_bus", "i2c0d", 3);
DECL_ENUMERATION("i2c_bus", "i2c0e", 4);
DECL_ENUMERATION("i2c_bus", "i2c0f", 5);
DECL_ENUMERATION("i2c_bus", "i2c0g", 6);
DECL_ENUMERATION("i2c_bus", "i2c0h", 7);
DECL_ENUMERATION("i2c_bus", "i2c1a", 8);
DECL_ENUMERATION("i2c_bus", "i2c1b", 9);
DECL_ENUMERATION("i2c_bus", "i2c1c", 10);
DECL_ENUMERATION("i2c_bus", "i2c1d", 11);
DECL_ENUMERATION("i2c_bus", "i2c1e", 12);
DECL_ENUMERATION("i2c_bus", "i2c1f", 13);
DECL_ENUMERATION("i2c_bus", "i2c1g", 14);
DECL_CONSTANT_STR("BUS_PINS_i2c0a", "gpio0,gpio1");
DECL_CONSTANT_STR("BUS_PINS_i2c0b", "gpio4,gpio5");
DECL_CONSTANT_STR("BUS_PINS_i2c0c", "gpio8,gpio9");
DECL_CONSTANT_STR("BUS_PINS_i2c0d", "gpio12,gpio13");
DECL_CONSTANT_STR("BUS_PINS_i2c0e", "gpio16,gpio17");
DECL_CONSTANT_STR("BUS_PINS_i2c0f", "gpio20,gpio21");
DECL_CONSTANT_STR("BUS_PINS_i2c0g", "gpio24,gpio25");
DECL_CONSTANT_STR("BUS_PINS_i2c0h", "gpio28,gpio29");
DECL_CONSTANT_STR("BUS_PINS_i2c1a", "gpio2,gpio3");
DECL_CONSTANT_STR("BUS_PINS_i2c1b", "gpio6,gpio7");
DECL_CONSTANT_STR("BUS_PINS_i2c1c", "gpio10,gpio11");
DECL_CONSTANT_STR("BUS_PINS_i2c1d", "gpio14,gpio15");
DECL_CONSTANT_STR("BUS_PINS_i2c1e", "gpio18,gpio19");
DECL_CONSTANT_STR("BUS_PINS_i2c1f", "gpio22,gpio23");
DECL_CONSTANT_STR("BUS_PINS_i2c1g", "gpio26,gpio27");

static const struct i2c_info i2c_bus[] = {
    { i2c0_hw,  0,  1, RESETS_RESET_I2C0_BITS },
    { i2c0_hw,  4,  5, RESETS_RESET_I2C0_BITS },
    { i2c0_hw,  8,  9, RESETS_RESET_I2C0_BITS },
    { i2c0_hw, 12, 13, RESETS_RESET_I2C0_BITS },
    { i2c0_hw, 16, 17, RESETS_RESET_I2C0_BITS },
    { i2c0_hw, 20, 21, RESETS_RESET_I2C0_BITS },
    { i2c0_hw, 24, 25, RESETS_RESET_I2C0_BITS },
    { i2c0_hw, 28, 29, RESETS_RESET_I2C0_BITS },

    { i2c1_hw,  2,  3, RESETS_RESET_I2C1_BITS },
    { i2c1_hw,  6,  7, RESETS_RESET_I2C1_BITS },
    { i2c1_hw, 10, 11, RESETS_RESET_I2C1_BITS },
    { i2c1_hw, 14, 15, RESETS_RESET_I2C1_BITS },
    { i2c1_hw, 18, 19, RESETS_RESET_I2C1_BITS },
    { i2c1_hw, 22, 23, RESETS_RESET_I2C1_BITS },
    { i2c1_hw, 26, 27, RESETS_RESET_I2C1_BITS },
};

struct i2c_config
i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
    if (bus > ARRAY_SIZE(i2c_bus))
        shutdown("Invalid i2c bus");

    const struct i2c_info *info = &i2c_bus[bus];

    gpio_peripheral(info->sda_pin, 3, 1);
    gpio_peripheral(info->scl_pin, 3, 1);

    if (!is_enabled_pclock(info->pclk)) {
        enable_pclock(info->pclk);

        i2c_hw_t *i2c = info->i2c;

        i2c->enable = 0;

        // We set up the bus in 400 kHz mode, but then set timings afterwards
        // to match either 100k or 400k mode. This simplifies the setup.

        i2c->con = I2C_IC_CON_SPEED_VALUE_FAST << I2C_IC_CON_SPEED_LSB
                 | I2C_IC_CON_MASTER_MODE_BITS
                 | I2C_IC_CON_IC_SLAVE_DISABLE_BITS
                 | I2C_IC_CON_IC_RESTART_EN_BITS;

        i2c->tx_tl = 0;
        i2c->rx_tl = 0;

        uint32_t pclk = get_pclock_frequency(info->pclk);

        // See `i2c_set_baudrate` in the Pico SDK `hardware_i2c/i2c.c` file
        // for details on the calculations here.
        if (rate > 1000000)
            rate = 1000000; // Clamp the rate to 1Mbps
        uint32_t period = (pclk + rate / 2) / rate;
        uint32_t lcnt = period * 3 / 5;
        uint32_t hcnt = period - lcnt;
        uint32_t sda_tx_hold_count = ((pclk * 3) / 10000000) + 1;

        i2c->fs_scl_hcnt = hcnt;
        i2c->fs_scl_lcnt = lcnt;
        i2c->fs_spklen = lcnt < 16 ? 1 : lcnt / 16;
        hw_write_masked(&i2c->sda_hold,
                        sda_tx_hold_count << I2C_IC_SDA_HOLD_IC_SDA_TX_HOLD_LSB,
                        I2C_IC_SDA_HOLD_IC_SDA_TX_HOLD_BITS);
    }

    return (struct i2c_config){ .i2c=info->i2c, .addr=addr };
}

static void
i2c_start(i2c_hw_t *i2c, uint8_t addr)
{
    i2c->enable = 0;
    i2c->tar = addr;
    i2c->enable = 1;
}

static void
i2c_stop(i2c_hw_t *i2c)
{
    i2c->enable = 0;
}

static void
i2c_do_write(i2c_hw_t *i2c, uint8_t addr, uint8_t write_len, uint8_t *write
             , uint8_t send_stop, uint32_t timeout)
{
    for (int i = 0; i < write_len; i++) {
        int first = i == 0;
        int last = send_stop && (i == write_len - 1);

        // Wait until there's a spot in the TX FIFO
        while (i2c->txflr == 16) {
            if (!timer_is_before(timer_read_time(), timeout))
                shutdown("i2c timeout");
        }

        i2c->data_cmd = first << I2C_IC_DATA_CMD_RESTART_LSB
                      | last << I2C_IC_DATA_CMD_STOP_LSB
                      | write[i];
    }

    if (!send_stop)
        return;

    // Drain the transmit buffer
    while (i2c->txflr != 0) {
        if (!timer_is_before(timer_read_time(), timeout))
            shutdown("i2c timeout");
    }
}

static void
i2c_do_read(i2c_hw_t *i2c, uint8_t addr, uint8_t read_len, uint8_t *read
            , uint32_t timeout)
{
    int have_read = 0;
    int to_send = read_len;
    while (have_read < read_len) {
        if (!timer_is_before(timer_read_time(), timeout))
            shutdown("i2c timeout");

        if (to_send > 0 && i2c->txflr < 16) {
            int first = to_send == read_len;
            int last = to_send == 1;

            // Put a read command in the TX FIFO
            i2c->data_cmd = first << I2C_IC_DATA_CMD_RESTART_LSB
                          | last << I2C_IC_DATA_CMD_STOP_LSB
                          | I2C_IC_DATA_CMD_CMD_BITS;
            to_send--;
        }

        if (have_read < read_len && i2c->rxflr > 0) {
            *read++ = i2c->data_cmd & 0xFF;
            have_read++;
        }
    }
}

void
i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
    i2c_hw_t *i2c = (i2c_hw_t*)config.i2c;
    uint32_t timeout = timer_read_time() + timer_from_us(5000);

    i2c_start(i2c, config.addr);
    i2c_do_write(i2c, config.addr, write_len, write, 1, timeout);
    i2c_stop(i2c);
}

void
i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
         , uint8_t read_len, uint8_t *read)
{
    i2c_hw_t *i2c = (i2c_hw_t*)config.i2c;
    uint32_t timeout = timer_read_time() + timer_from_us(5000);

    i2c_start(i2c, config.addr);
    if (reg_len != 0)
        i2c_do_write(i2c, config.addr, reg_len, reg, 0, timeout);
    i2c_do_read(i2c, config.addr, read_len, read, timeout);
    i2c_stop(i2c);
}
