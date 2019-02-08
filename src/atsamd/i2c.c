// i2c support on samd
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "internal.h" // enable_pclock
#include "command.h" // shutdown
#include "gpio.h" // i2c_setup
#include "sched.h" // sched_shutdown

#define TIME_RISE 125ULL // 125 nanoseconds
#define I2C_FREQ 100000

static void
i2c_init(void)
{
    static int have_run_init;
    if (have_run_init)
        return;
    have_run_init = 1;

    // Setup clock
    enable_pclock(SERCOM3_GCLK_ID_CORE, ID_SERCOM3);

    // Configure SDA, SCL pins
    gpio_peripheral(GPIO('A', 22), 'C', 0);
    gpio_peripheral(GPIO('A', 23), 'C', 0);

    // Configure i2c
    SercomI2cm *si = &SERCOM3->I2CM;
    si->CTRLA.reg = 0;
    uint32_t areg = (SERCOM_I2CM_CTRLA_LOWTOUTEN
                     | SERCOM_I2CM_CTRLA_INACTOUT(3)
                     | SERCOM_I2CM_STATUS_SEXTTOUT
                     | SERCOM_I2CM_STATUS_MEXTTOUT
                     | SERCOM_I2CM_CTRLA_MODE(5));
    si->CTRLA.reg = areg;
    uint32_t freq = get_pclock_frequency(SERCOM3_GCLK_ID_CORE);
    uint32_t baud = (freq/I2C_FREQ - 10 - freq*TIME_RISE/1000000000) / 2;
    si->BAUD.reg = baud;
    si->CTRLA.reg = areg | SERCOM_I2CM_CTRLA_ENABLE;
    while (si->SYNCBUSY.reg & SERCOM_I2CM_SYNCBUSY_ENABLE)
        ;

    // Go into idle mode
    si->STATUS.reg = SERCOM_I2CM_STATUS_BUSSTATE(1);
    while (si->SYNCBUSY.reg & SERCOM_I2CM_SYNCBUSY_SYSOP)
        ;
}

struct i2c_config
i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
    if (bus)
        shutdown("Unsupported i2c bus");
    i2c_init();
    return (struct i2c_config){ .addr=addr<<1 };
}

static void
i2c_wait(SercomI2cm *si)
{
    for (;;) {
        uint32_t intflag = si->INTFLAG.reg;
        if (!(intflag & SERCOM_I2CM_INTFLAG_MB)) {
            if (si->STATUS.reg & SERCOM_I2CM_STATUS_BUSERR)
                shutdown("i2c buserror");
            continue;
        }
        if (intflag & SERCOM_I2CM_INTFLAG_ERROR)
            shutdown("i2c error");
        break;
    }
}

static void
i2c_start(SercomI2cm *si, uint8_t addr)
{
    si->ADDR.reg = addr;
    i2c_wait(si);
}

static void
i2c_send_byte(SercomI2cm *si, uint8_t b)
{
    si->DATA.reg = b;
    i2c_wait(si);
}

static void
i2c_stop(SercomI2cm *si)
{
    si->CTRLB.reg = SERCOM_I2CM_CTRLB_CMD(3);
}

void
i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
    SercomI2cm *si = &SERCOM3->I2CM;
    i2c_start(si, config.addr);
    while (write_len--)
        i2c_send_byte(si, *write++);
    i2c_stop(si);
}

void
i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
         , uint8_t read_len, uint8_t *read)
{
    shutdown("i2c_read not supported on samd21");
}
