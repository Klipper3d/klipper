// Support for reading acceleration data from LSM6DSV16X over I2C
#include <string.h> // memcpy
#include "basecmd.h" // oid_alloc
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_COMMAND
#include "i2ccmds.h" // i2cdev_s
#include "sched.h" // DECL_TASK
#include "sensor_bulk.h" // sensor_bulk_report

#define REG_FIFO_STATUS1      0x1B
#define REG_FIFO_DATA_OUT_TAG 0x78
#define TAG_ACCEL_NC          0x02
#define BYTES_PER_FIFO_WORD   7
#define BYTES_PER_SAMPLE      6

struct lsm6dsv16x {
    struct timer timer;
    uint32_t rest_ticks;
    struct i2cdev_s *i2c;
    uint8_t flags;
    struct sensor_bulk sb;
};

enum {
    AX_HAVE_START = 1<<0,
    AX_RUNNING    = 1<<1,
};

static struct task_wake lsm6dsv16x_wake;

static uint_fast8_t
lsm6dsv16x_event(struct timer *timer)
{
    struct lsm6dsv16x *ax = container_of(timer, struct lsm6dsv16x, timer);
    uint32_t rest_ticks = ax->rest_ticks;
    uint8_t flags = ax->flags;
    if (flags & AX_HAVE_START) {
        sched_wake_task(&lsm6dsv16x_wake);
        flags &= ~AX_HAVE_START;
        ax->flags = flags;
    }
    ax->timer.waketime += rest_ticks;
    return SF_RESCHEDULE;
}

void
command_config_lsm6dsv16x(uint32_t *args)
{
    struct lsm6dsv16x *ax = oid_alloc(args[0], command_config_lsm6dsv16x, sizeof(*ax));
    ax->timer.func = lsm6dsv16x_event;
    ax->i2c = i2cdev_oid_lookup(args[1]);
}
DECL_COMMAND(command_config_lsm6dsv16x, "config_lsm6dsv16x oid=%c i2c_oid=%c");

void
command_query_lsm6dsv16x(uint32_t *args)
{
    struct lsm6dsv16x *ax = oid_lookup(args[0], command_config_lsm6dsv16x);
    sched_del_timer(&ax->timer);
    ax->flags = 0;
    if (!args[1])
        // End measurements
        return;

    // Start new measurements query
    ax->rest_ticks = args[1];
    sensor_bulk_reset(&ax->sb);
    irq_disable();
    ax->timer.waketime = timer_read_time() + ax->rest_ticks;
    sched_add_timer(&ax->timer);
    ax->flags = AX_HAVE_START | AX_RUNNING;
    irq_enable();
}
DECL_COMMAND(command_query_lsm6dsv16x, "query_lsm6dsv16x oid=%c rest_ticks=%u");

void
command_query_lsm6dsv16x_status(uint32_t *args)
{
    struct lsm6dsv16x *ax = oid_lookup(args[0], command_config_lsm6dsv16x);
    uint8_t reg = REG_FIFO_STATUS1;
    uint8_t status[2];
    uint32_t time1 = timer_read_time();
    int ret = i2c_read(ax->i2c->i2c_config, sizeof(reg), &reg, sizeof(status), status);
    uint32_t time2 = timer_read_time();

    uint16_t fifo_words = 0;
    if (!ret) {
        fifo_words = status[0] | ((status[1] & 0x03) << 8);
        if (status[1] & 0x40)
            ax->sb.possible_overflows++;
    }
    sensor_bulk_status(&ax->sb, args[0], time1, time2, fifo_words * BYTES_PER_SAMPLE);
}
DECL_COMMAND(command_query_lsm6dsv16x_status, "query_lsm6dsv16x_status oid=%c");

static void
lsm6dsv16x_query(struct lsm6dsv16x *ax, uint8_t oid)
{
    uint8_t reg_status = REG_FIFO_STATUS1;
    uint8_t status[2];
    if (i2c_read(ax->i2c->i2c_config, 1, &reg_status, 2, status) != 0)
        return;

    uint16_t unread_words = status[0] | ((status[1] & 0x03) << 8);
    if (status[1] & 0x40)
        ax->sb.possible_overflows++;
    if (!unread_words)
        return;

    // Read up to 6 words (42 bytes) per task pass to keep I2C transactions short
    uint8_t words_to_read = unread_words > 6 ? 6 : unread_words;
    uint8_t reg_data = REG_FIFO_DATA_OUT_TAG;
    uint8_t raw_buf[6 * BYTES_PER_FIFO_WORD];
    uint8_t read_bytes = words_to_read * BYTES_PER_FIFO_WORD;

    if (i2c_read(ax->i2c->i2c_config, 1, &reg_data, read_bytes, raw_buf) != 0)
        return;

    // Strip the 1-byte FIFO tag and pack 6-byte XYZ samples into sensor_bulk
    for (uint8_t i = 0; i < words_to_read; i++) {
        uint8_t *word = &raw_buf[i * BYTES_PER_FIFO_WORD];
        uint8_t sensor_tag = (word[0] >> 3) & 0x1F;
        if (sensor_tag == TAG_ACCEL_NC) {
            memcpy(&ax->sb.data[ax->sb.data_count], &word[1], BYTES_PER_SAMPLE);
            ax->sb.data_count += BYTES_PER_SAMPLE;
            if (ax->sb.data_count + BYTES_PER_SAMPLE > ARRAY_SIZE(ax->sb.data))
                sensor_bulk_report(&ax->sb, oid);
        }
    }

    // Reschedule immediately if more words remain in FIFO
    if (unread_words > words_to_read) {
        ax->flags |= AX_HAVE_START;
        sched_wake_task(&lsm6dsv16x_wake);
    } else {
        ax->flags |= AX_HAVE_START;
    }
}

void
lsm6dsv16x_task(void)
{
    if (!sched_check_wake(&lsm6dsv16x_wake))
        return;
    uint8_t oid;
    struct lsm6dsv16x *ax;
    foreach_oid(oid, ax, command_config_lsm6dsv16x) {
        uint_fast8_t flags = ax->flags;
        if (!(flags & AX_RUNNING))
            continue;
        if (flags & AX_HAVE_START) {
            ax->flags = flags & ~AX_HAVE_START;
            lsm6dsv16x_query(ax, oid);
        }
    }
}
DECL_TASK(lsm6dsv16x_task);
