// Support for gathering acceleration data from LIS2DW chip
//
// Copyright (C) 2023  Zhou.XianMing <zhouxm@biqu3d.com>
// Copyright (C) 2020-2025  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "autoconf.h" // CONFIG_WANT_SPI
#include "board/gpio.h" // irq_disable
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK
#include "sensor_bulk.h" // sensor_bulk_report
#include "spicmds.h" // spidev_transfer
#include "i2ccmds.h" // i2cdev_s

#define LIS_AR_DATAX0 0x28
#define LIS_AM_READ   0x80
#define LIS_MS_SPI    0x40
#define LIS_MS_I2C    0x80
#define LIS_FIFO_SAMPLES 0x2F

#define BYTES_PER_SAMPLE 6
#define BYTES_PER_BLOCK 48

struct lis2dw {
    struct timer timer;
    uint32_t rest_ticks;
    union {
        struct spidev_s *spi;
        struct i2cdev_s *i2c;
    };
    uint8_t bus_type;
    uint8_t flags;
    uint8_t model;
    uint8_t fifo_bytes_pending;
    struct sensor_bulk sb;
};

enum {
    LIS_PENDING = 1<<0,
};

enum {
    SPI_SERIAL, I2C_SERIAL,
};

DECL_ENUMERATION("bus_oid_type", "spi", SPI_SERIAL);
DECL_ENUMERATION("bus_oid_type", "i2c", I2C_SERIAL);

enum {
    LIS2DW, LIS3DH,
};

DECL_ENUMERATION("lis_chip_type", "LIS2DW", LIS2DW);
DECL_ENUMERATION("lis_chip_type", "LIS3DH", LIS3DH);

static struct task_wake lis2dw_wake;

// Event handler that wakes lis2dw_task() periodically
static uint_fast8_t
lis2dw_event(struct timer *timer)
{
    struct lis2dw *ax = container_of(timer, struct lis2dw, timer);
    ax->flags |= LIS_PENDING;
    sched_wake_task(&lis2dw_wake);
    return SF_DONE;
}

void
command_config_lis2dw(uint32_t *args)
{
    struct lis2dw *ax = oid_alloc(args[0], command_config_lis2dw
                                   , sizeof(*ax));
    ax->timer.func = lis2dw_event;

    switch (args[2]) {
        case SPI_SERIAL:
            if (CONFIG_WANT_SPI) {
                ax->spi = spidev_oid_lookup(args[1]);
                ax->bus_type = SPI_SERIAL;
                break;
            } else {
                shutdown("bus_type spi unsupported");
            }
        case I2C_SERIAL:
            if (CONFIG_WANT_I2C) {
               ax->i2c = i2cdev_oid_lookup(args[1]);
               ax->bus_type = I2C_SERIAL;
               break;
            } else {
                shutdown("bus_type i2c unsupported");
            }
        default:
            shutdown("bus_type invalid");
    }

    switch (args[3]) {
        case LIS2DW:
        case LIS3DH:
            ax->model = args[3];
            break;
        default:
            shutdown("model type invalid");
    }
}
DECL_COMMAND(command_config_lis2dw, "config_lis2dw oid=%c"
                " bus_oid=%c bus_oid_type=%c lis_chip_type=%c");

// Helper code to reschedule the lis2dw_event() timer
static void
lis2dw_reschedule_timer(struct lis2dw *ax)
{
    irq_disable();
    ax->timer.waketime = timer_read_time() + ax->rest_ticks;
    sched_add_timer(&ax->timer);
    irq_enable();
}

// Update local status tracking from newly read fifo status register
static void
update_fifo_status(struct lis2dw *ax, uint8_t fifo_status)
{
    if (fifo_status & 0x40)
        ax->sb.possible_overflows++;

    uint_fast8_t pending;
    if (ax->model == LIS3DH) {
        if (fifo_status & 0x20)
            pending = 0;
        else
            pending = fifo_status & 0x1F;
    } else {
        pending = fifo_status & 0x3F;
    }
    ax->fifo_bytes_pending = pending * BYTES_PER_SAMPLE;
}

// Query fifo status register
static void
query_fifo_status(struct lis2dw *ax)
{
    uint8_t fifo_status = 0;
    if (CONFIG_WANT_SPI && ax->bus_type == SPI_SERIAL) {
        uint8_t fifo[2] = { LIS_FIFO_SAMPLES | LIS_AM_READ, 0x00 };
        spidev_transfer(ax->spi, 1, sizeof(fifo), fifo);
        fifo_status = fifo[1];
    } else if (CONFIG_WANT_I2C && ax->bus_type == I2C_SERIAL) {
        uint8_t fifo_reg[1] = {LIS_FIFO_SAMPLES};
        int ret = i2c_dev_read(ax->i2c, sizeof(fifo_reg), fifo_reg
                               , sizeof(fifo_status), &fifo_status);
        i2c_shutdown_on_err(ret);
    }
    update_fifo_status(ax, fifo_status);
}

// Read 8 samples from FIFO via SPI
static void
read_fifo_block_spi(struct lis2dw *ax)
{
    uint8_t msg[BYTES_PER_BLOCK + 1] = {0};
    msg[0] = LIS_AR_DATAX0 | LIS_AM_READ;
    if (ax->model == LIS3DH)
        msg[0] |= LIS_MS_SPI;

    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    memcpy(ax->sb.data, &msg[1], BYTES_PER_BLOCK);
}

// Read 8 samples from FIFO via i2c
static void
read_fifo_block_i2c(struct lis2dw *ax)
{
    uint8_t msg_reg[] = {LIS_AR_DATAX0};
    if (ax->model == LIS3DH)
        msg_reg[0] |= LIS_MS_I2C;

    int ret = i2c_dev_read(ax->i2c, sizeof(msg_reg), msg_reg
                           , BYTES_PER_BLOCK, ax->sb.data);
    i2c_shutdown_on_err(ret);
}

// Read from fifo and transmit data to host
static void
read_fifo_block(struct lis2dw *ax, uint8_t oid)
{
    if (CONFIG_WANT_SPI && ax->bus_type == SPI_SERIAL)
        read_fifo_block_spi(ax);
    else if (CONFIG_WANT_I2C && ax->bus_type == I2C_SERIAL)
        read_fifo_block_i2c(ax);
    ax->sb.data_count = BYTES_PER_BLOCK;
    sensor_bulk_report(&ax->sb, oid);
    ax->fifo_bytes_pending -= BYTES_PER_BLOCK;
}

// Query accelerometer data
static void
lis2dw_query(struct lis2dw *ax, uint8_t oid)
{
    if (ax->fifo_bytes_pending < BYTES_PER_BLOCK)
        query_fifo_status(ax);

    if (ax->fifo_bytes_pending >= BYTES_PER_BLOCK)
        read_fifo_block(ax, oid);

    // check if we need to run the task again (more packets in fifo?)
    if (ax->fifo_bytes_pending >= BYTES_PER_BLOCK) {
        // More data in fifo - wake this task again
        sched_wake_task(&lis2dw_wake);
    } else {
        // Sleep until next check time
        ax->flags &= ~LIS_PENDING;
        lis2dw_reschedule_timer(ax);
    }
}

void
command_query_lis2dw(uint32_t *args)
{
    struct lis2dw *ax = oid_lookup(args[0], command_config_lis2dw);

    sched_del_timer(&ax->timer);
    ax->flags = 0;
    if (!args[1])
        // End measurements
        return;

    // Start new measurements query
    ax->rest_ticks = args[1];
    ax->fifo_bytes_pending = 0;
    sensor_bulk_reset(&ax->sb);
    lis2dw_reschedule_timer(ax);
}
DECL_COMMAND(command_query_lis2dw, "query_lis2dw oid=%c rest_ticks=%u");

void
command_query_lis2dw_status(uint32_t *args)
{
    struct lis2dw *ax = oid_lookup(args[0], command_config_lis2dw);
    uint32_t time1 = 0;
    uint32_t time2 = 0;
    uint8_t fifo_status = 0;

    if (CONFIG_WANT_SPI && ax->bus_type == SPI_SERIAL) {
        uint8_t fifo[2] = { LIS_FIFO_SAMPLES | LIS_AM_READ, 0x00 };
        time1 = timer_read_time();
        spidev_transfer(ax->spi, 1, sizeof(fifo), fifo);
        time2 = timer_read_time();
        fifo_status = fifo[1];
    } else if (CONFIG_WANT_I2C && ax->bus_type == I2C_SERIAL) {
        uint8_t fifo_reg[1] = {LIS_FIFO_SAMPLES};
        time1 = timer_read_time();
        int ret = i2c_dev_read(ax->i2c, sizeof(fifo_reg), fifo_reg
                               , sizeof(fifo_status), &fifo_status);
        time2 = timer_read_time();
        i2c_shutdown_on_err(ret);
    }
    update_fifo_status(ax, fifo_status);

    sensor_bulk_status(&ax->sb, args[0], time1, time2-time1
                       , ax->fifo_bytes_pending);
}
DECL_COMMAND(command_query_lis2dw_status, "query_lis2dw_status oid=%c");

void
lis2dw_task(void)
{
    if (!sched_check_wake(&lis2dw_wake))
        return;
    uint8_t oid;
    struct lis2dw *ax;
    foreach_oid(oid, ax, command_config_lis2dw) {
        uint_fast8_t flags = ax->flags;
        if (flags & LIS_PENDING)
            lis2dw_query(ax, oid);
    }
}
DECL_TASK(lis2dw_task);
