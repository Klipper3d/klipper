// Basic support for ADS1118 SPI controlled ADCs
//
// Copyright (C) 2024 Christoph Nelles <klipper-fw@evilazrael.de>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "generic/misc.h"
#include "sched.h"   // DECL_TASK
#include "spicmds.h" // spidev_transfer

struct ads1118_sensor_config
{
    unsigned adc : 1;
    unsigned mux : 3;
    unsigned pga : 3;
    unsigned dr : 3;
};

struct ads1118_spi
{
    struct spidev_s *spi;
    struct task_wake task_wake;
    struct timer timer;
    uint32_t response_interval;
    int16_t last_temperature;
    uint8_t oid;
    uint8_t sensor_count;
    uint8_t current_sensor;
    struct ads1118_sensor_config sensor_configs[5];
};

static uint_fast8_t ads1118_event(struct timer *timer);

static void ads1118_transfer(struct ads1118_spi *spi)
{
    uint8_t current_sensor = spi->current_sensor;
    uint8_t next_sensor = current_sensor + 1;
    if (next_sensor == spi->sensor_count)
        next_sensor = 0;

    uint8_t msg[4] = {0, 0, 0, 0};
    msg[1] |= 0b1 << 3;
    msg[1] |= 0b01 << 1;
    msg[1] |= 1;
    if (spi->sensor_configs[next_sensor].adc == 0)
    {
        // 0 - ADC / 1 - Temperature
        msg[1] |= 0b1 << 4;
    }
    else
    {
        // mux setting
        msg[0] |= (spi->sensor_configs[next_sensor].mux & 0b111) << 4;
        // pga
        msg[0] |= (spi->sensor_configs[next_sensor].pga & 0b111) << 1;
    }
    msg[1] |= spi->sensor_configs[next_sensor].dr << 5;
    msg[2] = msg[0];
    msg[3] = msg[1];
    spidev_transfer(spi->spi, 1, 4, msg);
    if (spi->sensor_configs[current_sensor].adc == 0)
        spi->last_temperature = msg[0] << 8 | msg[1];
    sendf("ads1118_result oid=%u temperature=%c sensor=%c value=%c", spi->oid,
          spi->last_temperature, current_sensor, (msg[0] << 8) | msg[1]);
    spi->current_sensor = next_sensor;
}

void command_config_ads1118(uint32_t *args)
{
    struct ads1118_spi *spi =
        oid_alloc(args[0], command_config_ads1118, sizeof(*spi));
    spi->oid = args[0];
    spi->spi = spidev_oid_lookup(args[1]);
    spi->sensor_configs[0].adc = 0;
    spi->sensor_configs[0].dr = args[2] & 0b111;
    spi->sensor_count = 1;
    spi->response_interval = timer_from_us(1000) * args[3];
    spi->timer.func = ads1118_event;
    ads1118_transfer(spi);
    spi->timer.waketime = timer_read_time() + spi->response_interval;
    sched_add_timer(&spi->timer);
}
DECL_COMMAND(command_config_ads1118,
             "config_ads1118 oid=%u spi_oid=%u data_rate=%u"
             " response_interval=%u");

void command_add_sensor_ads1118(uint32_t *args)
{
    struct ads1118_spi *spi = oid_lookup(args[0], command_config_ads1118);
    if (spi->sensor_count == 5)
        shutdown("ads1118 too many sensors");
    spi->sensor_configs[spi->sensor_count].adc = 1;
    spi->sensor_configs[spi->sensor_count].mux = args[1] & 0b111;
    spi->sensor_configs[spi->sensor_count].pga = args[2] & 0b111;
    spi->sensor_configs[spi->sensor_count].dr = args[3] & 0b111;
    spi->sensor_count++;
}
DECL_COMMAND(command_add_sensor_ads1118,
             "add_sensor_ads1118 oid=%u mux=%u pga=%u dr=%u");

static uint_fast8_t ads1118_event(struct timer *timer)
{
    struct ads1118_spi *spi = container_of(timer, struct ads1118_spi, timer);
    sched_wake_task(&spi->task_wake);
    ads1118_transfer(spi);

    spi->timer.waketime = timer_read_time() + spi->response_interval;
    return SF_RESCHEDULE;
}
