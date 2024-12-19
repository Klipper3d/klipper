// Support for querying magnetic angle sensors via SPI
//
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/misc.h" // timer_read_time
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_disable
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK
#include "sensor_bulk.h" // sensor_bulk_report
#include "spicmds.h" // spidev_transfer

enum {
    SA_CHIP_A1333,
    SA_CHIP_AS5047D,
    SA_CHIP_TLE5012B,
    SA_CHIP_MT6816,
    SA_CHIP_MT6826S,
    SA_CHIP_MAX
};

DECL_ENUMERATION("spi_angle_type", "a1333", SA_CHIP_A1333);
DECL_ENUMERATION("spi_angle_type", "as5047d", SA_CHIP_AS5047D);
DECL_ENUMERATION("spi_angle_type", "tle5012b", SA_CHIP_TLE5012B);
DECL_ENUMERATION("spi_angle_type", "mt6816", SA_CHIP_MT6816);
DECL_ENUMERATION("spi_angle_type", "mt6826s", SA_CHIP_MT6826S);

enum { TCODE_ERROR = 0xff };
enum {
    SE_OVERFLOW, SE_SCHEDULE, SE_SPI_TIME, SE_CRC, SE_DUP, SE_NO_ANGLE
};

#define MAX_SPI_READ_TIME timer_from_us(50)

struct spi_angle {
    struct timer timer;
    uint32_t rest_ticks;
    struct spidev_s *spi;
    uint8_t flags, chip_type, time_shift, overflow;
    struct sensor_bulk sb;
};

enum {
    SA_PENDING = 1<<2,
};

#define BYTES_PER_SAMPLE 3

static struct task_wake angle_wake;

// Event handler that wakes spi_angle_task() periodically
static uint_fast8_t
angle_event(struct timer *timer)
{
    struct spi_angle *sa = container_of(timer, struct spi_angle, timer);
    uint8_t flags = sa->flags;
    if (sa->flags & SA_PENDING)
        sa->overflow++;
    else
        sa->flags = flags | SA_PENDING;
    sched_wake_task(&angle_wake);
    sa->timer.waketime += sa->rest_ticks;
    return SF_RESCHEDULE;
}

void
command_config_spi_angle(uint32_t *args)
{
    uint8_t chip_type = args[2];
    if (chip_type > SA_CHIP_MAX)
        shutdown("Invalid spi_angle chip type");
    struct spi_angle *sa = oid_alloc(args[0], command_config_spi_angle
                                     , sizeof(*sa));
    sa->timer.func = angle_event;
    sa->spi = spidev_oid_lookup(args[1]);
    if (!spidev_have_cs_pin(sa->spi))
        shutdown("angle sensor requires cs pin");
    sa->chip_type = chip_type;
}
DECL_COMMAND(command_config_spi_angle,
             "config_spi_angle oid=%c spi_oid=%c spi_angle_type=%c");

// Send spi_angle_data message if buffer is full
static void
angle_check_report(struct spi_angle *sa, uint8_t oid)
{
    if (sa->sb.data_count + BYTES_PER_SAMPLE > ARRAY_SIZE(sa->sb.data))
        sensor_bulk_report(&sa->sb, oid);
}

// Add an entry to the measurement buffer
static void
angle_add(struct spi_angle *sa, uint_fast8_t tcode, uint_fast16_t data)
{
    sa->sb.data[sa->sb.data_count] = tcode;
    sa->sb.data[sa->sb.data_count + 1] = data;
    sa->sb.data[sa->sb.data_count + 2] = data >> 8;
    sa->sb.data_count += BYTES_PER_SAMPLE;
}

// Add an error indicator to the measurement buffer
static void
angle_add_error(struct spi_angle *sa, uint_fast8_t error_code)
{
    angle_add(sa, TCODE_ERROR, error_code);
}

// Add a measurement to the buffer
static void
angle_add_data(struct spi_angle *sa, uint32_t stime, uint32_t mtime
               , uint_fast16_t angle)
{
    uint32_t tdiff = mtime - stime;
    if (sa->time_shift)
        tdiff = (tdiff + (1<<(sa->time_shift - 1))) >> sa->time_shift;
    if (tdiff >= TCODE_ERROR) {
        angle_add_error(sa, SE_SCHEDULE);
        return;
    }
    angle_add(sa, tdiff, angle);
}

// a1333 sensor query
static void
a1333_query(struct spi_angle *sa, uint32_t stime)
{
    uint8_t msg[2] = { 0x32, 0x00 };
    uint32_t mtime1 = timer_read_time();
    spidev_transfer(sa->spi, 1, sizeof(msg), msg);
    uint32_t mtime2 = timer_read_time();
    // Data is latched on first sclk edge of response
    if (mtime2 - mtime1 > MAX_SPI_READ_TIME)
        angle_add_error(sa, SE_SPI_TIME);
    else if (msg[0] & 0x80)
        angle_add_error(sa, SE_CRC);
    else
        angle_add_data(sa, stime, mtime1, (msg[0] << 9) | (msg[1] << 1));
}

static int bit_parity(uint8_t *msg)
{
    uint_fast8_t parity = msg[0] ^ msg[1];
    parity ^= parity >> 4;
    parity ^= parity >> 2;
    parity ^= parity >> 1;
    return parity;
}

// as5047d sensor query
static void
as5047d_query(struct spi_angle *sa, uint32_t stime)
{
    uint8_t msg[2] = { 0x7F, 0xFE };
    uint32_t mtime1 = timer_read_time();
    spidev_transfer(sa->spi, 0, sizeof(msg), msg);
    uint32_t mtime2 = timer_read_time();
    // Data is latched on CS pin rising after query request
    if (mtime2 - mtime1 > MAX_SPI_READ_TIME) {
        angle_add_error(sa, SE_SPI_TIME);
        return;
    }
    msg[0] = 0xC0;
    msg[1] = 0x00;
    spidev_transfer(sa->spi, 1, sizeof(msg), msg);
    uint_fast8_t parity = bit_parity(msg);
    if (parity & 1)
        angle_add_error(sa, SE_CRC);
    else if (msg[0] & 0x40)
        angle_add_error(sa, SE_NO_ANGLE);
    else
        angle_add_data(sa, stime, mtime2, (msg[0] << 10) | (msg[1] << 2));
}

static void mt6816_query(struct spi_angle *sa, uint32_t stime)
{
    uint8_t msg[3] = {0x83, 0x00, 0x00};
    uint32_t mtime1 = timer_read_time();
    spidev_transfer(sa->spi, 1, sizeof(msg), msg);
    uint32_t mtime2 = timer_read_time();
    // Data is latched on first sclk edge of response
    if (mtime2 - mtime1 > MAX_SPI_READ_TIME) {
        angle_add_error(sa, SE_SPI_TIME);
        return;
    }
    uint_fast8_t parity = bit_parity(&msg[1]);
    if (parity & 1)
        angle_add_error(sa, SE_CRC);
    else if (msg[2] & 0x02)
        angle_add_error(sa, SE_NO_ANGLE);
    else
        angle_add_data(sa, stime, mtime2, (msg[1] << 8) | (msg[2] & 0xfc));
}

static uint8_t
crc8_mt(uint8_t crc, uint8_t data)
{
    crc ^= data;
    int i;
    for (i = 0; i < 8; i++)
        crc = crc & 0x80 ? (crc << 1) ^ 0x07 : crc << 1;
    return crc;
}

static void mt6826s_query(struct spi_angle *sa, uint32_t stime)
{
    uint8_t msg[6] = {0x30, 0x03, 0x00, 0x00, 0x00, 0x00};
    uint32_t mtime1 = timer_read_time();
    spidev_transfer(sa->spi, 1, sizeof(msg), msg);
    uint32_t mtime2 = timer_read_time();
    // Data is latched on first sclk edge of response
    if (mtime2 - mtime1 > MAX_SPI_READ_TIME) {
        angle_add_error(sa, SE_SPI_TIME);
        return;
    }
    uint8_t crc = 0;
    for (int i = 2; i < 5; i++)
        crc = crc8_mt(crc, msg[i]);

    if (crc != msg[5])
        angle_add_error(sa, SE_CRC);
    else if (msg[4] & 0x02)
        angle_add_error(sa, SE_NO_ANGLE);
    else
        angle_add_data(sa, stime, mtime2, (msg[2] << 8) | msg[3]);
}


#define TLE_READ 0x80
#define TLE_READ_LATCH (TLE_READ | 0x04)
#define TLE_REG_AVAL 0x02

// crc8 "J1850" calculation for tle5012b messages
static uint8_t
crc8(uint8_t crc, uint8_t data)
{
    crc ^= data;
    int i;
    for (i=0; i<8; i++)
        crc = crc & 0x80 ? (crc << 1) ^ 0x1d : crc << 1;
    return crc;
}

// microsecond delay helper
static inline void
udelay(uint32_t usecs)
{
    uint32_t end = timer_read_time() + timer_from_us(usecs);
    while (!timer_is_before(end, timer_read_time()))
        irq_poll();
}

// tle5012b sensor query
static void
tle5012b_query(struct spi_angle *sa, uint32_t stime)
{
    struct gpio_out cs_pin = spidev_get_cs_pin(sa->spi);
    // Latch data (data is latched on rising CS of a NULL message)
    gpio_out_write(cs_pin, 0);
    udelay(1);
    irq_disable();
    gpio_out_write(cs_pin, 1);
    uint32_t mtime = timer_read_time();
    irq_enable();

    uint8_t msg[10] = { TLE_READ_LATCH, (TLE_REG_AVAL << 4) | 0x03 };
    uint8_t crc = 0x05; // 0x05 == crc8(crc8(0xff, msg[0]), msg[1])
    spidev_transfer(sa->spi, 1, sizeof(msg), msg);
    int i;
    for (i=2; i<8; i++)
        crc = crc8(crc, msg[i]);
    if (((~crc) & 0xff) != msg[9])
        angle_add_error(sa, SE_CRC);
    else if (!(msg[8] & (1<<4)))
        angle_add_error(sa, SE_NO_ANGLE);
    else if (!(msg[2] & 0x80))
        angle_add_error(sa, SE_DUP);
    else if (mtime - stime > timer_from_us(32 * 32 * 1000000UL / 750000))
        angle_add_error(sa, SE_SCHEDULE);
    else
        angle_add(sa, (msg[6] >> 1) & 0x3f, (msg[2] << 9) | (msg[3] << 1));
}

void
command_query_spi_angle(uint32_t *args)
{
    uint8_t oid = args[0];
    struct spi_angle *sa = oid_lookup(oid, command_config_spi_angle);

    sched_del_timer(&sa->timer);
    sa->flags = 0;
    if (!args[2])
        // End measurements
        return;

    // Start new measurements query
    sa->timer.waketime = args[1];
    sa->rest_ticks = args[2];
    sensor_bulk_reset(&sa->sb);
    sa->time_shift = args[3];
    sched_add_timer(&sa->timer);
}
DECL_COMMAND(command_query_spi_angle,
             "query_spi_angle oid=%c clock=%u rest_ticks=%u time_shift=%c");

void
command_spi_angle_transfer(uint32_t *args)
{
    uint8_t oid = args[0];
    struct spi_angle *sa = oid_lookup(oid, command_config_spi_angle);
    uint8_t data_len = args[1];
    uint8_t *data = command_decode_ptr(args[2]);
    uint32_t mtime;
    uint_fast8_t chip = sa->chip_type;
    if (chip == SA_CHIP_TLE5012B) {
        // Latch data (data is latched on rising CS of a NULL message)
        struct gpio_out cs_pin = spidev_get_cs_pin(sa->spi);
        gpio_out_write(cs_pin, 0);
        udelay(1);
        irq_disable();
        gpio_out_write(cs_pin, 1);
        mtime = timer_read_time();
        irq_enable();
        spidev_transfer(sa->spi, 1, data_len, data);
    } else {
        uint32_t mtime1 = timer_read_time();
        spidev_transfer(sa->spi, 1, data_len, data);
        uint32_t mtime2 = timer_read_time();
        if (mtime2 - mtime1 > MAX_SPI_READ_TIME)
            data_len = 0;
        if (chip == SA_CHIP_AS5047D)
            mtime = mtime2;
        else
            mtime = mtime1;
    }
    sendf("spi_angle_transfer_response oid=%c clock=%u response=%*s"
          , oid, mtime, data_len, data);
}
DECL_COMMAND(command_spi_angle_transfer, "spi_angle_transfer oid=%c data=%*s");

// Background task that performs measurements
void
spi_angle_task(void)
{
    if (!sched_check_wake(&angle_wake))
        return;
    uint8_t oid;
    struct spi_angle *sa;
    foreach_oid(oid, sa, command_config_spi_angle) {
        uint_fast8_t flags = sa->flags;
        if (!(flags & SA_PENDING))
            continue;
        irq_disable();
        uint32_t stime = sa->timer.waketime;
        uint_fast8_t overflow = sa->overflow;
        sa->flags = 0;
        sa->overflow = 0;
        irq_enable();
        stime -= sa->rest_ticks;
        while (overflow--) {
            angle_add_error(sa, SE_OVERFLOW);
            angle_check_report(sa, oid);
        }
        uint_fast8_t chip = sa->chip_type;
        if (chip == SA_CHIP_A1333)
            a1333_query(sa, stime);
        else if (chip == SA_CHIP_AS5047D)
            as5047d_query(sa, stime);
        else if (chip == SA_CHIP_TLE5012B)
            tle5012b_query(sa, stime);
        else if (chip == SA_CHIP_MT6816)
            mt6816_query(sa, stime);
        else if (chip == SA_CHIP_MT6826S)
            mt6826s_query(sa, stime);
        angle_check_report(sa, oid);
    }
}
DECL_TASK(spi_angle_task);
