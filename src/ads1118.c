// Support for ADS1118 ADC with up to two thermocouples
//
// Copyright (C) 2022  Jacob Dockter <dockterj@gmail.com>
// Largely based on code from src/thermocouple.c
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include <stdbool.h>
#include "board/irq.h" // irq_disable
#include "basecmd.h" // oid_alloc
#include "byteorder.h" // be32_to_cpu
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK
#include "spicmds.h" // spidev_transfer

struct thermocouple_spi {
    struct timer timer;
    uint32_t rest_time;
    uint8_t chan_a_oid;
    uint8_t chan_b_oid;
    int16_t chan_a_min_value;           // Min allowed ADC value
    int16_t chan_a_max_value;           // Max allowed ADC value
    int16_t chan_b_min_value;           // Min allowed ADC value
    int16_t chan_b_max_value;           // Max allowed ADC value
    int16_t adc_chan_a_mv, adc_chan_b_mv, cj_temp;
    struct spidev_s *spi;
    uint8_t max_invalid, invalid_count;
    uint8_t flags;
    uint8_t state, cj_skip_count;
};

enum {
    TS_PENDING = 1,
};

#define ADS1118_COLD_JUNCTION_HIGH_FAULT 0x01
#define ADS1118_COLD_JUNCTION_LOW_FAULT 0x02
#define ADS1118_CONFIG_READ_ERROR 0x04

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
    struct thermocouple_spi *spi = oid_alloc(
        args[0], command_config_ads1118, sizeof(*spi));
    spi->timer.func = thermocouple_event;
    spi->spi = spidev_oid_lookup(args[1]);
    sched_del_timer(&spi->timer);
    spi->timer.waketime = args[2];
    spi->rest_time = args[3];
    if (! spi->rest_time)
        return;
    spi->max_invalid = args[4];
    spi->invalid_count = 0;
    sched_add_timer(&spi->timer);
}
DECL_COMMAND(command_config_ads1118,
             "config_ads1118 oid=%c spi_oid=%c clock=%u rest_ticks=%u "
             "max_invalid_count=%c");

void
command_config_ads1118_channel(uint32_t *args)
{
    struct thermocouple_spi *spi = oid_lookup(
        args[4], command_config_ads1118);
    if (args[1] == 0) {
        spi->chan_a_oid = args[0];
        spi->chan_a_min_value = args[2];
        spi->chan_a_max_value = args[3];
    } else if (args[1] == 1) {
        spi->chan_b_oid = args[0];
        spi->chan_b_min_value = args[2];
        spi->chan_b_max_value = args[3];
    } else
        shutdown("Invalid ADS1118 pin number");
}
DECL_COMMAND(command_config_ads1118_channel,
             "config_ads1118_channel oid=%c pin_number=%c min_sample_value=%hi "
             " max_sample_value=%hi parent_oid=%c");

static void
ads1118_respond(struct thermocouple_spi *spi, uint32_t next_begin_time
                     , int16_t adc_mv, int16_t cj_temp, uint8_t fault
                     , uint8_t oid, int16_t min_value, int16_t max_value)
{
    if (spi->cj_temp > 4000)
        fault |= ADS1118_COLD_JUNCTION_HIGH_FAULT;
    if (spi->cj_temp < -1280)
        fault |= ADS1118_COLD_JUNCTION_LOW_FAULT;

    sendf("ads1118_result oid=%c next_clock=%u adc_mv=%hi cj_temp=%hi fault=%c",
          oid, next_begin_time, adc_mv, cj_temp, fault);
    /* check the result and stop if below or above allowed range */
    if (fault || adc_mv < min_value || adc_mv > max_value) {
        //output fault data as a warning?
        spi->invalid_count++;
        if (spi->invalid_count < spi->max_invalid
            && !(fault & ADS1118_CONFIG_READ_ERROR))
            return;
        try_shutdown("Thermocouple reader fault");
    }

    // no fault - so reset invalid_count
    spi->invalid_count = 0;
}

static void
thermocouple_handle_ads1118(struct thermocouple_spi *spi
                             , uint32_t next_begin_time, uint8_t oid)
{
    uint8_t msg[4];
    uint8_t cur_state = spi->state;
    uint8_t next_state;
    uint8_t fault = 0;

    //todo check if conversion is available, if not set fault
    next_state = cur_state + 1;

    // only set next_state to 2 or 3 if those channels are configured
    if (spi->chan_a_oid == 0 && spi->chan_b_oid == 0)
        next_state = 1;
    else if (cur_state == 1 && spi->chan_a_oid == 0)
        next_state = 3;
    else if (cur_state == 2 && spi->chan_b_oid == 0)
        next_state = 4;

    // loop around to state 1, but only read the cold junction every 10th
    // iteration
    if (next_state == 4) {
        next_state = 1;
        spi->cj_skip_count += 1;
        if (spi->cj_skip_count < 9)
            next_state = 2;
        else
            spi->cj_skip_count = 0;
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

    // check that the config bytes returned match our config
    if (msg[2] != (int16_t)be32_to_cpu(value) >> 8 ||
        msg[3] != (int8_t)be32_to_cpu(value)) {
        fault |= ADS1118_CONFIG_READ_ERROR;
        output("ads1118_value msg1=%hi msg2=%hi b1=%hi b2=%hi",
           (int16_t)msg[2],
           (int16_t)msg[3],
           (int16_t)be32_to_cpu(value) >> 8,
           (int8_t)be32_to_cpu(value) );
    }

    // discard the config bytes
    value = be32_to_cpu(value) >> 16;

    // value read will be for the cur_state
    if (cur_state == 0) {
        // init - read cold junction next time around
    } else if (cur_state == 1) {
        // cold junction temperature is returned in a 14 bit
        // signed (two's compliment) int
        spi->cj_temp = ((int16_t)value) / 4;
    } else if (cur_state == 2) {
        spi->adc_chan_a_mv = (int16_t)value;
        ads1118_respond(spi, next_begin_time, spi->adc_chan_a_mv, spi->cj_temp,
            fault, spi->chan_a_oid, spi->chan_a_min_value,
            spi->chan_a_max_value);
    } else if (cur_state == 3) {
        spi->adc_chan_b_mv = (int16_t)value;
        ads1118_respond(spi, next_begin_time, spi->adc_chan_b_mv, spi->cj_temp,
            fault, spi->chan_b_oid, spi->chan_b_min_value,
            spi->chan_b_max_value);
    }
    spi->state = next_state;
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
        thermocouple_handle_ads1118(spi, next_begin_time, oid);
    }
}
DECL_TASK(ads1118_task);
