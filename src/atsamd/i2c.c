// i2c support on samd
//
// Copyright (C) 2019  Florian Heilmann <Florian.Heilmann@gmx.net>
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "internal.h" // enable_pclock
#include "command.h" // shutdown
#include "gpio.h" // i2c_setup
#include "sched.h" // sched_shutdown
#include "i2ccmds.h" // I2C_BUS_SUCCESS

#define TIME_RISE 125ULL // 125 nanoseconds
#define I2C_FREQ      100000
#define I2C_FREQ_FAST 400000

static void
i2c_init(uint32_t bus, uint32_t rate, SercomI2cm *si)
{
    static uint8_t have_run_init;
    if (have_run_init & (1<<bus))
        return;
    have_run_init |= 1<<bus;

    // Configure i2c
    si->CTRLA.reg = 0;
    uint32_t areg = (SERCOM_I2CM_CTRLA_LOWTOUTEN
                     | SERCOM_I2CM_CTRLA_INACTOUT(3)
                     | SERCOM_I2CM_CTRLA_MODE(5));
    si->CTRLA.reg = areg;
    uint32_t freq = sercom_get_pclock_frequency(bus);
    uint32_t baud = 0;
    if (rate < I2C_FREQ_FAST) {
        baud = (freq/I2C_FREQ - 10 - freq*TIME_RISE/1000000000) / 2;
    } else {
        baud = (freq/I2C_FREQ_FAST - 10 - freq*TIME_RISE/1000000000) / 2;
    }
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
    Sercom *sercom = sercom_enable_pclock(bus);
    sercom_i2c_pins(bus);
    SercomI2cm *si = &sercom->I2CM;
    i2c_init(bus, rate, si);
    return (struct i2c_config){ .si=si, .addr=addr<<1 };
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

int
i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
    SercomI2cm *si = (SercomI2cm *)config.si;
    i2c_start(si, config.addr);
    while (write_len--)
        i2c_send_byte(si, *write++);
    i2c_stop(si);

    return I2C_BUS_SUCCESS;
}

int
i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
         , uint8_t read_len, uint8_t *read)
{
    SercomI2cm *si = (SercomI2cm *)config.si;

    // start in write mode and write register if provided
    if(reg_len) {
        // start in write mode
        si->ADDR.reg = config.addr;
        while (!(si->INTFLAG.reg & SERCOM_I2CM_INTFLAG_MB));

        // write registers
        while (reg_len--){
            si->DATA.reg = *reg++;
            while (!(si->INTFLAG.reg & SERCOM_I2CM_INTFLAG_MB));
        }
    }

    // start with read bit enabled
    si->ADDR.reg = (config.addr | 0x1);

    // read bytes from slave
    while (read_len--){
        while (!(si->INTFLAG.reg & SERCOM_I2CM_INTFLAG_SB));

        if (read_len){
            // set ACK response
            si->CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;
            while (si->SYNCBUSY.bit.SYSOP);

            // execute ACK succeded by byte read
            si->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(2);
            while (si->SYNCBUSY.bit.SYSOP);
        } else {
            // set NACK response
            si->CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT;
            while (si->SYNCBUSY.bit.SYSOP);

            // execute NACK succeded by stop condition
            si->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
            while (si->SYNCBUSY.bit.SYSOP);
        }

        // read received data byte
        *read++ = si->DATA.reg;
    }

    return I2C_BUS_SUCCESS;
}
