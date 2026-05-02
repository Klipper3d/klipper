// Support for ADS131M02 ADC Chip
//
// Copyright (C) 2026  Dmitry Butyugin <dmbutyugin@google.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h>
#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_in_setup
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_add_timer
#include "sensor_bulk.h" // sensor_bulk_report
#include "spicmds.h" // spidev_transfer
#include "trigger_analog.h" // trigger_analog_update

struct ads131m02_adc {
    struct timer timer;
    uint32_t rest_ticks;
    struct gpio_in data_ready;
    struct spidev_s *spi;
    uint8_t pending_flag, data_count;
    uint8_t channel;
    struct sensor_bulk sb;
    struct trigger_analog *ta;
};

#define BYTES_PER_SAMPLE 4
#define ADS131M02_FRAME_SIZE 12

static struct task_wake wake_ads131m02;

/****************************************************************
 * ADS131M02 Sensor Support
 ****************************************************************/

int8_t
ads131m02_is_data_ready(struct ads131m02_adc *ads131m02) {
    return gpio_in_read(ads131m02->data_ready) == 0;
}

// Event handler that periodically wakes the capture task
static uint_fast8_t
ads131m02_event(struct timer *timer)
{
    struct ads131m02_adc *ads131m02 = container_of(
            timer, struct ads131m02_adc, timer);
    uint32_t rest_ticks = ads131m02->rest_ticks;
    if (ads131m02->pending_flag) {
        ads131m02->sb.possible_overflows++;
        rest_ticks *= 4;
    } else if (ads131m02_is_data_ready(ads131m02)) {
        ads131m02->pending_flag = 1;
        sched_wake_task(&wake_ads131m02);
        rest_ticks *= 8;
    }
    ads131m02->timer.waketime += rest_ticks;
    return SF_RESCHEDULE;
}

// Send a NULL command frame and read conversion data from both channels.
// Returns sample from the configured channel.
static void
ads131m02_read_adc(struct ads131m02_adc *ads131m02, uint8_t oid)
{
    uint8_t msg[ADS131M02_FRAME_SIZE] = {0};
    spidev_transfer(ads131m02->spi, 1, sizeof(msg), msg);
    ads131m02->pending_flag = 0;
    barrier();

    // Frame layout: status(3), ch0(3), ch1(3), crc(3)
    // Data is 24-bit two's complement, MSB first
    uint32_t raw;
    if (ads131m02->channel == 0) {
        raw = ((uint32_t)msg[3] << 16)
            | ((uint32_t)msg[4] << 8)
            | ((uint32_t)msg[5]);
    } else {
        raw = ((uint32_t)msg[6] << 16)
            | ((uint32_t)msg[7] << 8)
            | ((uint32_t)msg[8]);
    }

    // Extend 24-bit two's complement to 32-bit
    if (raw & 0x800000)
        raw |= 0xFF000000;

    trigger_analog_update(ads131m02->ta, raw);

    // Add sample to bulk buffer
    ads131m02->sb.data[ads131m02->sb.data_count] = raw;
    ads131m02->sb.data[ads131m02->sb.data_count + 1] = raw >> 8;
    ads131m02->sb.data[ads131m02->sb.data_count + 2] = raw >> 16;
    ads131m02->sb.data[ads131m02->sb.data_count + 3] = raw >> 24;
    ads131m02->sb.data_count += BYTES_PER_SAMPLE;

    if ((ads131m02->sb.data_count + BYTES_PER_SAMPLE) >
            ARRAY_SIZE(ads131m02->sb.data)) {
        sensor_bulk_report(&ads131m02->sb, oid);
    }
}

// Create an ads131m02 sensor
void
command_config_ads131m02(uint32_t *args)
{
    struct ads131m02_adc *ads131m02 = oid_alloc(args[0]
                 , command_config_ads131m02, sizeof(*ads131m02));
    ads131m02->timer.func = ads131m02_event;
    ads131m02->pending_flag = 0;
    ads131m02->spi = spidev_oid_lookup(args[1]);
    ads131m02->channel = (uint8_t)(args[2] & 0xFF);
    ads131m02->data_ready = gpio_in_setup(args[3], 0);
}
DECL_COMMAND(command_config_ads131m02, "config_ads131m02 oid=%c"
    " spi_oid=%c channel=%c data_ready_pin=%u");

void
ads131m02_attach_trigger_analog(uint32_t *args) {
    uint8_t oid = args[0];
    struct ads131m02_adc *ads131m02 = oid_lookup(oid, command_config_ads131m02);
    ads131m02->ta = trigger_analog_oid_lookup(args[1]);
}
#if CONFIG_WANT_TRIGGER_ANALOG
DECL_COMMAND(ads131m02_attach_trigger_analog,
    "ads131m02_attach_trigger_analog oid=%c trigger_analog_oid=%c");
#endif

// Start/stop capturing ADC data
void
command_query_ads131m02(uint32_t *args)
{
    uint8_t oid = args[0];
    struct ads131m02_adc *ads131m02 = oid_lookup(oid, command_config_ads131m02);
    sched_del_timer(&ads131m02->timer);
    ads131m02->pending_flag = 0;
    ads131m02->rest_ticks = args[1];
    if (!ads131m02->rest_ticks) {
        // End measurements
        return;
    }
    // Start new measurements
    sensor_bulk_reset(&ads131m02->sb);
    irq_disable();
    ads131m02->timer.waketime = timer_read_time() + ads131m02->rest_ticks;
    sched_add_timer(&ads131m02->timer);
    irq_enable();
}
DECL_COMMAND(command_query_ads131m02, "query_ads131m02 oid=%c rest_ticks=%u");

void
command_query_ads131m02_status(const uint32_t *args)
{
    uint8_t oid = args[0];
    struct ads131m02_adc *ads131m02 = oid_lookup(oid, command_config_ads131m02);
    irq_disable();
    const uint32_t start_t = timer_read_time();
    uint8_t is_data_ready = ads131m02_is_data_ready(ads131m02);
    irq_enable();
    uint8_t pending_bytes = is_data_ready ? BYTES_PER_SAMPLE : 0;
    sensor_bulk_status(&ads131m02->sb, oid, start_t, 0, pending_bytes);
}
DECL_COMMAND(command_query_ads131m02_status, "query_ads131m02_status oid=%c");

// Background task that performs measurements
void
ads131m02_capture_task(void)
{
    if (!sched_check_wake(&wake_ads131m02))
        return;
    uint8_t oid;
    struct ads131m02_adc *ads131m02;
    foreach_oid(oid, ads131m02, command_config_ads131m02) {
        if (ads131m02->pending_flag)
            ads131m02_read_adc(ads131m02, oid);
    }
}
DECL_TASK(ads131m02_capture_task);
