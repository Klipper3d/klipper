// I2C functions on lpc176x
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "LPC17xx.h" // LPC_I2C1
#include "board/misc.h" // timer_is_before
#include "command.h" // DECL_COMMAND
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown

// i2c connection status flags
enum {
    IF_START = 1<<5, IF_STOP = 1<<4, IF_IRQ = 1<<3, IF_ACK = 1<<2, IF_ENA = 1<<6
};

static void
i2c_init(void)
{
    static int have_run_init;
    if (have_run_init)
        return;
    have_run_init = 1;

    // Init i2c bus 1 pins
    gpio_peripheral(0, 0, 3, 0);
    gpio_peripheral(0, 1, 3, 0);

    // Set 100Khz frequency
    uint32_t PCLK = SystemCoreClock / 4, pulse = PCLK / (100000 * 2);
    LPC_I2C1->I2SCLL = pulse;
    LPC_I2C1->I2SCLH = pulse;

    // Enable interface
    LPC_I2C1->I2CONCLR = IF_START | IF_IRQ | IF_ACK | IF_ENA;
    LPC_I2C1->I2CONSET = IF_ENA;
}

static void
i2c_wait(uint32_t bit, uint32_t timeout)
{
    for (;;) {
        uint32_t flags = LPC_I2C1->I2CONSET;
        if (flags & bit)
            break;
        if (!timer_is_before(timer_read_time(), timeout))
            shutdown("i2c timeout");
    }
}

static void
i2c_start(uint32_t timeout)
{
    LPC_I2C1->I2CONCLR = IF_ACK | IF_IRQ | IF_START;
    LPC_I2C1->I2CONSET = IF_ACK | IF_START;
    i2c_wait(IF_IRQ, timeout);
    uint32_t status = LPC_I2C1->I2STAT;
    if (status != 0x10 && status != 0x08)
        shutdown("Failed to send i2c start");
    LPC_I2C1->I2CONCLR = IF_START;
}

static uint32_t
i2c_send_byte(uint8_t b, uint32_t timeout)
{
    LPC_I2C1->I2DAT = b;
    LPC_I2C1->I2CONCLR = IF_IRQ;
    i2c_wait(IF_IRQ, timeout);
    return LPC_I2C1->I2STAT;
}

static void
i2c_stop(uint32_t timeout)
{
    LPC_I2C1->I2CONSET = IF_STOP;
    LPC_I2C1->I2CONCLR = IF_IRQ;
    i2c_wait(IF_STOP, timeout);
}

static void
i2c_send(uint8_t *data, int data_len)
{
    i2c_init();
    uint32_t timeout = timer_read_time() + timer_from_us(5000);

    i2c_start(timeout);
    while (data_len--)
        i2c_send_byte(*data++, timeout);
    i2c_stop(timeout);
}

// This provides just enough functionality to program an MCP4451 chip
void
command_i2c_send(uint32_t *args)
{
    uint8_t data_len = args[0], *data = (void*)(size_t)args[1];
    i2c_send(data, data_len);
}
DECL_COMMAND(command_i2c_send, "i2c_send data=%*s");
