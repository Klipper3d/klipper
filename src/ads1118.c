// Support for ADS1118 ADC with up to two thermocouples
//
// Copyright (C) 2022  Jacob Dockter <dockterj@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "board/irq.h" // irq_disable
#include "basecmd.h" // oid_alloc
#include "byteorder.h" // be32_to_cpu
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK
#include "spicmds.h" // spidev_transfer

enum {
    TS_CHIP_ADS1118A, TS_CHIP_ADS1118B
};

DECL_ENUMERATION("thermocouple_type", "ADS1118A", TS_CHIP_ADS1118A);
DECL_ENUMERATION("thermocouple_type", "ADS1118B", TS_CHIP_ADS1118B);

struct thermocouple_spi {
    struct timer timer;
    uint32_t rest_time;
    uint32_t min_value;           // Min allowed ADC value
    uint32_t max_value;           // Max allowed ADC value
    uint32_t value;
    struct spidev_s *spi;
    uint8_t chip_type, flags;
};

enum {
    TS_PENDING = 1,
};

uint32_t ads_cj;
uint32_t ads_t1;
uint32_t ads_t2;
uint16_t cj_loop;
uint8_t has_b;
uint8_t state;

static struct task_wake thermocouple_wake;

static uint_fast8_t thermocouple_event(struct timer *timer) {
    struct thermocouple_spi *spi = container_of(
            timer, struct thermocouple_spi, timer);
    // Trigger task to read and send results
    sched_wake_task(&thermocouple_wake);
    spi->flags |= TS_PENDING;
    spi->timer.waketime += spi->rest_time;
    return SF_RESCHEDULE;
}

void
command_config_ads1118(uint32_t *args)
{
    uint8_t chip_type = args[2];
    if (chip_type > TS_CHIP_ADS1118B)
        shutdown("Invalid thermocouple chip type");

    struct thermocouple_spi *spi = oid_alloc(
        args[0], command_config_ads1118, sizeof(*spi));
    spi->timer.func = thermocouple_event;
    // only ads1118a communicates to the chip via spi
    if (chip_type == TS_CHIP_ADS1118A) {
        spi->spi = spidev_oid_lookup(args[1]);
    }
    spi->chip_type = chip_type;

    if (chip_type == TS_CHIP_ADS1118B) {
        has_b = 1;
    }
}
DECL_COMMAND(command_config_ads1118,
             "config_ads1118 oid=%c spi_oid=%c thermocouple_type=%c");

void
command_query_ads1118(uint32_t *args)
{
    sendf("ads1118_query oid=%c type=%c",
          args[0], args[1]);
    struct thermocouple_spi *spi = oid_lookup(
        args[0], command_config_ads1118);

    sched_del_timer(&spi->timer);
    spi->timer.waketime = args[1];
    spi->rest_time = args[2];
    if (! spi->rest_time)
        return;
    spi->min_value = args[3];
    spi->max_value = args[4];
    sched_add_timer(&spi->timer);
}
DECL_COMMAND(command_query_ads1118,
             "query_ads1118 oid=%c clock=%u rest_ticks=%u"
             " min_value=%u max_value=%u");

static void
ads1118_respond(struct thermocouple_spi *spi, uint32_t next_begin_time
                     , uint32_t value, uint32_t value2, uint8_t fault
                     , uint8_t oid)
{
    sendf("ads1118_result oid=%c next_clock=%u value=%u value2=%u fault=%c",
          oid, next_begin_time, value, value2, fault);
    /* check the result and stop if below or above allowed range */
    if (value < spi->min_value || value > spi->max_value) {
        sendf("thermocouple_out_of_range oid=%c next_clock=%u value=%u "
               "min=%u max=%u", oid, next_begin_time, value, spi->min_value,
               spi->max_value);
        //try_shutdown("Thermocouple ADC out of range");
    }
}

static void
thermocouple_handle_ads1118(struct thermocouple_spi *spi
                             , uint32_t next_begin_time, uint8_t oid)
{
    uint8_t msg[4];
    uint8_t cur_state = state;
    uint8_t next_state;

    next_state = cur_state + 1;
    // only set next_state to 3 (ads1118b) if configured
    if (cur_state == 2 && !has_b) {
        next_state = 4;
    }

    // loop around to state 1, but only read the cold junction every 10th iteration
    if (next_state == 4) {
        next_state = 1;
        cj_loop += 1;
        if (cj_loop < 9) {
            next_state = 2;
        } else {
            cj_loop = 0;
        }
    }

    // set the ads1118 to read the next_state
    if (next_state == 1) {
        // set next reading to be cold junction
        // start conversion, AIN0, AIN1, .256V, single shot
        msg[0] = 0b10001101;
        // 64 SPS, cold junction mode, pullup disabled, update config, reserved
        msg[1] = 0b01110010;
    } else if (next_state == 2) {
        // set next reading to ADS1118A (AIN0, AIN1)
        // start conversion, AIN0, AIN1, .256V, single shot
        msg[0] = 0b10001101;
        // 64 SPS, ADC mode, pullup disabled, update config, reserved
        msg[1] = 0b01100010;
    } else if (next_state == 3) {
        // set next reading to ADS1118B (AIN2, AIN3)
        // start conversion, AIN2, AIN3, .256V, single shot
        msg[0] = 0b10111101;
        // 64 SPS, ADC mode, pullup disabled, update config, reserved
        msg[1] = 0b01100010;
    }
    // send message twice
    msg[2] = msg[0];
    msg[3] = msg[1];

    spidev_transfer(spi->spi, 1, sizeof(msg), msg);
    uint32_t value;
    memcpy(&value, msg, sizeof(value));
    value = be32_to_cpu(value) >> 16;

    // value will be for the cur_state
    if (cur_state == 0) {
        // init - read cold junction next time around
    } else if (cur_state == 1) {
        uint32_t value1 = value >> 2;
        ads_cj = value1;
        sendf("thermocouple_result_cj oid=%c next_clock=%u value=%u state=%c",
               oid, next_begin_time, value1, state);
    } else if (cur_state == 2) {
        sendf("thermocouple_result_ads1118a oid=%c next_clock=%u value=%u state=%c",
               oid, next_begin_time, value, state);

        ads_t1 = value;
    } else if (cur_state == 3) {
        sendf("thermocouple_result_ads1118b oid=%c next_clock=%u value=%u state=%c",
               oid, next_begin_time, value, state);

        ads_t2 = value;
    }
    state = next_state;

    // Kill after data send, host decode an error
    //if (value & 0x04)
        //try_shutdown("Thermocouple reader fault");
}

// task to read thermocouple and send response
void
ads1118_task(void)
{
    if (!sched_check_wake(&thermocouple_wake))
        return;
    uint8_t oid;
    struct thermocouple_spi *spi;
    foreach_oid(oid, spi, command_config_ads1118) {
        if (!(spi->flags & TS_PENDING))
            continue;
        irq_disable();
        uint32_t next_begin_time = spi->timer.waketime;
        spi->flags &= ~TS_PENDING;
        irq_enable();
        switch (spi->chip_type) {
        case TS_CHIP_ADS1118A:
            thermocouple_handle_ads1118(spi, next_begin_time, oid);
            if (ads_t1 > 0) {
                ads1118_respond(spi, next_begin_time, ads_t1, ads_cj, 0, oid);
                ads_t1 = 0;
            }
            break;
        case TS_CHIP_ADS1118B:
            if (ads_t2 > 0) {
                ads1118_respond(spi, next_begin_time, ads_t2, ads_cj, 0, oid);
                ads_t2 = 0;
            }
            break;
        }
    }
}
DECL_TASK(ads1118_task);
