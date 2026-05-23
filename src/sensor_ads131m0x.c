// Support for ADS131M0x ADC Chips
//
// Copyright (C) 2024-2025 Gareth Farrington <gareth@waves.ky>
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

struct ads131m0x_adc {
    struct timer timer;
    uint32_t rest_ticks;
    struct gpio_in data_ready;
    struct spidev_s *spi;
    uint8_t pending_flag, data_count, was_reset;
    uint8_t channel, num_channels;
    struct sensor_bulk sb;
    struct trigger_analog *ta;
};

// Internal errors transmitted to trigger_analog
enum {
    SE_ERROR_CRC = 1,
    SE_ERROR_RESET = 2
};

DECL_ENUMERATION("ads131m0x_error:", "SENSOR_ERROR_CRC", SE_ERROR_CRC);
DECL_ENUMERATION("ads131m0x_error:", "SENSOR_ERROR_RESET", SE_ERROR_RESET);

#define BYTES_PER_SAMPLE 4
#define ADS131M0X_WORD_SIZE 3
#define ADS131M0X_STATUS_WORDS 1
#define ADS131M0X_CRC_WORDS 1
#define ADS131M0X_MAX_CHANNELS 4
#define ADS131M0X_MAX_FRAME_SIZE \
    ((ADS131M0X_STATUS_WORDS + ADS131M0X_MAX_CHANNELS \
        + ADS131M0X_CRC_WORDS) * ADS131M0X_WORD_SIZE)

// Bit 10 of 16-bit status, in byte 0 (bits 15:8)
#define STATUS_RESET_BIT 4

static struct task_wake wake_ads131m0x;

static void
buffer_append_int32(struct sensor_bulk *sb, int32_t val)
{
    sb->data[sb->data_count] = val;
    sb->data[sb->data_count + 1] = val >> 8;
    sb->data[sb->data_count + 2] = val >> 16;
    sb->data[sb->data_count + 3] = val >> 24;
    sb->data_count += BYTES_PER_SAMPLE;
}

/****************************************************************
 * ADS131M0X Sensor Support
 ****************************************************************/

static inline uint_fast8_t
ads131m0x_is_data_ready(struct ads131m0x_adc *ads131m0x) {
    return gpio_in_read(ads131m0x->data_ready) == 0;
}

// CCITT CRC-16 calculation (polynomial 0x1021, init 0xFFFF, optimized version)
static uint16_t
ads131m0x_crc16_ccitt(const uint8_t *data, uint8_t len)
{
    uint16_t crc = 0xFFFF;
    while (len--) {
        uint8_t x = (crc >> 8) ^ *data++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t)x << 12) ^ ((uint16_t)x << 5)
            ^ (uint16_t)x;
    }
    return crc;
}

static inline uint_fast8_t
ads131m0x_has_crc_error(uint8_t *msg, uint8_t frame_size)
{
    uint_fast8_t crc_offset = frame_size - ADS131M0X_WORD_SIZE;
    uint16_t calc_crc = ads131m0x_crc16_ccitt(msg, crc_offset);
    uint16_t frame_crc = ((uint16_t)msg[crc_offset] << 8) | msg[crc_offset + 1];
    return calc_crc != frame_crc;
}

// Event handler that periodically wakes the capture task
static uint_fast8_t
ads131m0x_event(struct timer *timer)
{
    struct ads131m0x_adc *ads131m0x = container_of(
            timer, struct ads131m0x_adc, timer);
    uint32_t rest_ticks = ads131m0x->rest_ticks;
    if (ads131m0x->pending_flag) {
        ads131m0x->sb.possible_overflows++;
        rest_ticks *= 4;
    } else if (ads131m0x_is_data_ready(ads131m0x)) {
        ads131m0x->pending_flag = 1;
        sched_wake_task(&wake_ads131m0x);
        rest_ticks *= 8;
    }
    ads131m0x->timer.waketime += rest_ticks;
    return SF_RESCHEDULE;
}

static void
ads131m0x_flush_buffer(struct ads131m0x_adc *ads131m0x, uint8_t oid)
{
    if ((ads131m0x->sb.data_count + BYTES_PER_SAMPLE) >
            ARRAY_SIZE(ads131m0x->sb.data)) {
        sensor_bulk_report(&ads131m0x->sb, oid);
    }
}

static void
ads131m0x_publish_error(struct ads131m0x_adc *ads131m0x, uint8_t oid
                        , uint8_t error_code)
{
    trigger_analog_note_error(ads131m0x->ta, error_code);
    int32_t err_value = (int32_t)((uint32_t)error_code << 24);
    buffer_append_int32(&ads131m0x->sb, err_value);
    ads131m0x_flush_buffer(ads131m0x, oid);
}

// Send a NULL command frame and read conversion data from all channels.
// Returns sample from the configured channel.
static void
ads131m0x_read_adc(struct ads131m0x_adc *ads131m0x, uint8_t oid)
{
    uint8_t frame_size = (ADS131M0X_STATUS_WORDS + ads131m0x->num_channels
                          + ADS131M0X_CRC_WORDS) * ADS131M0X_WORD_SIZE;
    uint8_t msg[ADS131M0X_MAX_FRAME_SIZE] = {0};
    spidev_transfer(ads131m0x->spi, 1, frame_size, msg);
    ads131m0x->pending_flag = 0;
    barrier();

    // Validate CRC
    if (ads131m0x_has_crc_error(msg, frame_size)) {
        ads131m0x_publish_error(ads131m0x, oid, SE_ERROR_CRC);
        return;
    }

    // Check for unexpected sensor reset
    if (msg[0] & STATUS_RESET_BIT) {
        ads131m0x->was_reset = 1;
    }
    if (ads131m0x->was_reset) {
        ads131m0x_publish_error(ads131m0x, oid, SE_ERROR_RESET);
        return;
    }

    // Frame layout: status(3), channel[0](3), channel[1](3), ..., crc(3)
    // Data is 24-bit two's complement, MSB first
    uint8_t data_offset = ADS131M0X_STATUS_WORDS * ADS131M0X_WORD_SIZE
        + ads131m0x->channel * ADS131M0X_WORD_SIZE;
    uint32_t raw = ((uint32_t)msg[data_offset] << 16)
        | ((uint32_t)msg[data_offset + 1] << 8)
        | ((uint32_t)msg[data_offset + 2]);

    // Extend 24-bit two's complement to 32-bit
    if (raw & 0x800000)
        raw |= 0xFF000000;

    trigger_analog_update(ads131m0x->ta, raw);
    buffer_append_int32(&ads131m0x->sb, raw);
    ads131m0x_flush_buffer(ads131m0x, oid);
}

// Create an ads131m0x sensor
void
command_config_ads131m0x(uint32_t *args)
{
    struct ads131m0x_adc *ads131m0x = oid_alloc(args[0]
            , command_config_ads131m0x, sizeof(*ads131m0x));
    ads131m0x->timer.func = ads131m0x_event;
    ads131m0x->pending_flag = 0;
    ads131m0x->spi = spidev_oid_lookup(args[1]);
    ads131m0x->channel = (uint8_t)(args[2] & 0xFF);
    ads131m0x->num_channels = (uint8_t)(args[3] & 0xFF);
    if (ads131m0x->num_channels < 2 ||
            ads131m0x->num_channels > ADS131M0X_MAX_CHANNELS ||
            ads131m0x->channel >= ads131m0x->num_channels)
        shutdown("Invalid ads131m0x channels configuration");
    ads131m0x->data_ready = gpio_in_setup(args[4], 0);
}
DECL_COMMAND(command_config_ads131m0x, "config_ads131m0x oid=%c"
    " spi_oid=%c channel=%c num_channels=%c data_ready_pin=%u");

void
ads131m0x_attach_trigger_analog(uint32_t *args) {
    uint8_t oid = args[0];
    struct ads131m0x_adc *ads131m0x = oid_lookup(oid, command_config_ads131m0x);
    ads131m0x->ta = trigger_analog_oid_lookup(args[1]);
}
#if CONFIG_WANT_TRIGGER_ANALOG
DECL_COMMAND(ads131m0x_attach_trigger_analog,
    "ads131m0x_attach_trigger_analog oid=%c trigger_analog_oid=%c");
#endif

// Start/stop capturing ADC data
void
command_query_ads131m0x(uint32_t *args)
{
    uint8_t oid = args[0];
    struct ads131m0x_adc *ads131m0x = oid_lookup(oid, command_config_ads131m0x);
    sched_del_timer(&ads131m0x->timer);
    ads131m0x->pending_flag = 0;
    ads131m0x->rest_ticks = args[1];
    if (!ads131m0x->rest_ticks) {
        // End measurements
        return;
    }
    // Start new measurements
    ads131m0x->was_reset = 0;
    sensor_bulk_reset(&ads131m0x->sb);
    irq_disable();
    ads131m0x->timer.waketime = timer_read_time() + ads131m0x->rest_ticks;
    sched_add_timer(&ads131m0x->timer);
    irq_enable();
}
DECL_COMMAND(command_query_ads131m0x, "query_ads131m0x oid=%c rest_ticks=%u");

void
command_query_ads131m0x_status(const uint32_t *args)
{
    uint8_t oid = args[0];
    struct ads131m0x_adc *ads131m0x = oid_lookup(oid, command_config_ads131m0x);
    irq_disable();
    const uint32_t start_t = timer_read_time();
    uint8_t is_data_ready = ads131m0x_is_data_ready(ads131m0x);
    irq_enable();
    uint8_t pending_bytes = is_data_ready ? BYTES_PER_SAMPLE : 0;
    sensor_bulk_status(&ads131m0x->sb, oid, start_t, 0, pending_bytes);
}
DECL_COMMAND(command_query_ads131m0x_status, "query_ads131m0x_status oid=%c");

// Background task that performs measurements
void
ads131m0x_capture_task(void)
{
    if (!sched_check_wake(&wake_ads131m0x))
        return;
    uint8_t oid;
    struct ads131m0x_adc *ads131m0x;
    foreach_oid(oid, ads131m0x, command_config_ads131m0x) {
        if (ads131m0x->pending_flag)
            ads131m0x_read_adc(ads131m0x, oid);
    }
}
DECL_TASK(ads131m0x_capture_task);
