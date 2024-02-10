// Support for gathering acceleration data from ADXL345 chip using I2C
//
// Copyright (C) 2020-2023  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2024       Endrik Einberg <endrik@endrik.dev>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK
#include "sensor_bulk.h" // sensor_bulk_report
#include "board/gpio.h" // i2c_read
#include "i2ccmds.h" // i2cdev_oid_lookup

// Definitions for ADXL345 chip registers
#define AR_DATAX0      0x32 // Data register for X-Axis
#define AR_FIFO_STATUS 0x39 // FIFO status register
#define SET_FIFO_CTL   0x90 // FIFO control register
#define BYTES_PER_SAMPLE 5  // Bytes per accelerometer sample

// Struct to store ADXL345 data and configuration
struct adxl345_i2c {
    struct timer timer; // Timer for scheduling reads
    uint32_t rest_ticks; // Ticks to rest between reads
    struct i2cdev_s *i2c; // I2C device pointer
    uint16_t fifo_entries; // FIFO entries available
    uint8_t flags; // Status flags
    struct sensor_bulk sb; // Sensor bulk data storage
};

// Flag definitions
enum {
    AX_PENDING = 1 << 0, // Data read pending flag
};

static struct task_wake adxl345_i2c_wake; // Task wake-up management

// Event handler to wake up the adxl345_task periodically
static uint_fast8_t adxl345_i2c_event(struct timer *timer) {
    struct adxl345_i2c *ax2 = container_of(timer, struct adxl345_i2c, timer);
    ax2->flags |= AX_PENDING;
    sched_wake_task(&adxl345_i2c_wake);
    return SF_DONE;
}

// Initialize ADXL345 sensor configuration
void command_config_adxl345_i2c(uint32_t *args) {
    struct adxl345_i2c *ax2 = oid_alloc(args[0],
        command_config_adxl345_i2c, sizeof(*ax2));
    ax2->timer.func = adxl345_i2c_event;
    ax2->i2c = i2cdev_oid_lookup(args[1]);
}
DECL_COMMAND(command_config_adxl345_i2c,
             "config_adxl345_i2c oid=%c i2c_oid=%c");

// Reschedules the timer for the next data read
static void adxl_i2c_reschedule_timer(struct adxl345_i2c *ax2) {
    irq_disable();
    ax2->timer.waketime = timer_read_time() + ax2->rest_ticks;
    sched_add_timer(&ax2->timer);
    irq_enable();
}

// Queries accelerometer data and handles errors
static void adxl_i2c_query(struct adxl345_i2c *ax2, uint8_t oid) {
    uint8_t reg = AR_DATAX0; // Start register for accelerometer data
    uint8_t data[8]; // Buffer for reading data
    i2c_read(ax2->i2c->i2c_config, 1, &reg, sizeof(data), data);
    ax2->fifo_entries = data[7] & ~0x80; // Mask off trigger bit

    uint8_t *d = &ax2->sb.data[ax2->sb.data_count];

    // Check for data errors before processing
    if (((data[1] & 0xf0) && (data[1] & 0xf0) != 0xf0)
        || ((data[3] & 0xf0) && (data[3] & 0xf0) != 0xf0)
        || ((data[5] & 0xf0) && (data[5] & 0xf0) != 0xf0)
        || (data[6] != SET_FIFO_CTL) || (ax2->fifo_entries > 32)) {
        // Fill with error code and reset FIFO entries
        memset(d, 0xff, BYTES_PER_SAMPLE);
        ax2->fifo_entries = 0;
    } else {
        // Process and store valid X, Y, Z readings
        d[0] = data[0]; // X low
        d[1] = data[2]; // Y low
        d[2] = data[4]; // Z low
        d[3] = (data[1] & 0x1f) | (data[5] << 5); // X high and Z high
        d[4] = (data[3] & 0x1f) | ((data[5] << 2) & 0x60); // Y high and Z high
    }

    ax2->sb.data_count += BYTES_PER_SAMPLE;
    if (ax2->sb.data_count + BYTES_PER_SAMPLE > ARRAY_SIZE(ax2->sb.data))
        sensor_bulk_report(&ax2->sb, oid);

    // Manage FIFO data and reschedule as needed
    if (ax2->fifo_entries >= 31)
        ax2->sb.possible_overflows++;
    if (ax2->fifo_entries > 1) {
        sched_wake_task(&adxl345_i2c_wake);
    } else {
        ax2->flags &= ~AX_PENDING;
        adxl_i2c_reschedule_timer(ax2);
    }
}

// Start or stop data queries for ADXL345
void command_query_adxl345_i2c(uint32_t *args) {
    struct adxl345_i2c *ax2 = oid_lookup(args[0], command_config_adxl345_i2c);

    sched_del_timer(&ax2->timer);
    ax2->flags = 0;
    if (!args[1])
        return; // Stop measurements

    // Begin new measurement cycle
    ax2->rest_ticks = args[1];
    sensor_bulk_reset(&ax2->sb);
    adxl_i2c_reschedule_timer(ax2);
}
DECL_COMMAND(command_query_adxl345_i2c,
             "query_adxl345_i2c oid=%c rest_ticks=%u");

// Query the status of the ADXL345 sensor
void command_query_adxl345_i2c_status(uint32_t *args) {
    struct adxl345_i2c *ax2 = oid_lookup(args[0], command_config_adxl345_i2c);
    uint8_t msg[1] = { AR_FIFO_STATUS };

    uint32_t time1 = timer_read_time();
    uint8_t fifo_status_msg;
    i2c_read(ax2->i2c->i2c_config, 1, msg, 1, &fifo_status_msg);
    uint32_t time2 = timer_read_time();

    uint_fast8_t fifo_status = fifo_status_msg & ~0x80; // Mask off trigger bit
    if (fifo_status > 32)
        return; // Error in query, no response sent

    sensor_bulk_status(&ax2->sb,
                       args[0],
                       time1,
                       time2 - time1,
                       fifo_status * BYTES_PER_SAMPLE);
}
DECL_COMMAND(command_query_adxl345_i2c_status,
             "query_adxl345_i2c_status oid=%c");

// Task to handle ADXL345 data queries
void adxl345_i2c_task(void) {
    if (!sched_check_wake(&adxl345_i2c_wake))
        return;
    uint8_t oid;
    struct adxl345_i2c *ax2;
    foreach_oid(oid, ax2, command_config_adxl345_i2c) {
        if (ax2->flags & AX_PENDING)
            adxl_i2c_query(ax2, oid);
    }
}
DECL_TASK(adxl345_i2c_task);
