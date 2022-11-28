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

enum {
    TS_CHIP_ADS1118A, TS_CHIP_ADS1118B
};

DECL_ENUMERATION("thermocouple_type", "ADS1118A", TS_CHIP_ADS1118A);
DECL_ENUMERATION("thermocouple_type", "ADS1118B", TS_CHIP_ADS1118B);

struct thermocouple_spi {
    struct timer timer;
    uint32_t rest_time;
    int16_t min_value;           // Min allowed ADC value
    int16_t max_value;           // Max allowed ADC value
    int16_t adc_chan_a_mv, adc_chan_b_mv, cj_temp;
    bool new_reading_chan_a;
    bool new_reading_chan_b;
    struct spidev_s *spi;
    uint8_t max_invalid, invalid_count;
    uint8_t chip_type, flags;
    uint8_t state, cj_loop;
};

enum {
    TS_PENDING = 1,
};

//todo - store these in appropriate spi?
//  create and store all values in chan A spi
//  don't process chan b in these loops, but
//  send the response when read in chan A
//  chan A = initiator, main, primary, parent
//  chan B = device, replica, secondary, child
int16_t cj_temp;
int16_t adcA_mv;
int16_t adcB_mv;
//todo - store these in chan A spi
bool has_b;

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
        has_b = true;
    }
}
DECL_COMMAND(command_config_ads1118,
             "config_ads1118 oid=%c spi_oid=%c thermocouple_type=%c");

void
command_query_ads1118(uint32_t *args)
{
    struct thermocouple_spi *spi = oid_lookup(
        args[0], command_config_ads1118);

    sched_del_timer(&spi->timer);
    spi->timer.waketime = args[1];
    spi->rest_time = args[2];
    if (! spi->rest_time)
        return;
    spi->min_value = args[3];
    spi->max_value = args[4];
    spi->max_invalid = args[5];
    spi->invalid_count = 0;
    sched_add_timer(&spi->timer);
}
DECL_COMMAND(command_query_ads1118,
             "query_ads1118 oid=%c clock=%u rest_ticks=%u"
             " min_value=%u max_value=%u");

static void
ads1118_respond(struct thermocouple_spi *spi, uint32_t next_begin_time
                     , int16_t adc_mv, int16_t cj_temp, uint8_t fault
                     , uint8_t oid)
{
    sendf("ads1118_result oid=%c next_clock=%u adc_mv=%hi cj_temp=%hi fault=%c",
          oid, next_begin_time, adc_mv, cj_temp, fault);
    /* check the result and stop if below or above allowed range */
    if (fault || adc_mv < spi->min_value || adc_mv > spi->max_value) {
        spi->invalid_count++;
        if (spi->invalid_count < spi->max_invalid)
            return;
        try_shutdown("Thermocouple reader fault");
    }
    spi->invalid_count = 0;
}

static void
thermocouple_handle_ads1118(struct thermocouple_spi *spi
                             , uint32_t next_begin_time, uint8_t oid)
{
    //todo - check if conversion is ready before reading
    uint8_t msg[4];
    uint8_t cur_state = spi->state;
    uint8_t next_state;

    next_state = cur_state + 1;
    // only set next_state to 3 (ads1118b) if configured
    if (cur_state == 2 && !has_b) {
        next_state = 4;
    }

    // loop around to state 1, but only read the cold junction every 10th
    // or 20th  iteration
    if (next_state == 4) {
        next_state = 1;
        spi->cj_loop += 1;
        if (spi->cj_loop < 9) {
            next_state = 2;
        } else {
            spi->cj_loop = 0;
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

    // value read will be for the cur_state
    if (cur_state == 0) {
        // init - read cold junction next time around
    } else if (cur_state == 1) {
        // cold junction temperature is returned in a 14 bit
        // signed (two's compliment) int
        // todo - test conversion for negative cj_temp values
        value = value >> 2;
        //if (value && 0x2000)
            //value = value - (1 << 14);
        cj_temp = (int16_t)value;
        spi->cj_temp = (int16_t)value;
        output("ads1118_result oid=%c cj_temp=%hi",
          oid, cj_temp);
    } else if (cur_state == 2) {
        adcA_mv = (int16_t)value;
        spi->adc_chan_a_mv = (int16_t)value;
        spi->new_reading_chan_a = true;
        output("ads1118_result_a oid=%c adc_mv=%hi",
          oid, adcA_mv);
    } else if (cur_state == 3) {
        adcB_mv = (int16_t)value;
        spi->adc_chan_b_mv = (int16_t)value;
        spi->new_reading_chan_b = true;
        output("ads1118_result_b oid=%c adc_mv=%hi",
          oid, adcB_mv);
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
        switch (spi->chip_type) {
        case TS_CHIP_ADS1118A:
            thermocouple_handle_ads1118(spi, next_begin_time, oid);
            //todo - set fault somewhere for overrange, underrange, open circuit
            if (spi->new_reading_chan_a) {
                ads1118_respond(spi, next_begin_time, adcA_mv, cj_temp, 0, oid);
                spi->new_reading_chan_a = false;
            }
            break;
        case TS_CHIP_ADS1118B:
            if (spi->new_reading_chan_b) {
                ads1118_respond(spi, next_begin_time, adcB_mv, cj_temp, 0, oid);
                spi->new_reading_chan_b = false;
            }
            break;
        }
    }
}
DECL_TASK(ads1118_task);
