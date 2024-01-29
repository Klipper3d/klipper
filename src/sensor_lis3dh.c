
#include <string.h> // memcpy
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK
#include "sensor_bulk.h" // sensor_bulk_report
#include "spicmds.h" // spidev_transfer

#define LIS_AR_DATAX0 0x28
#define LIS_AM_READ   0x80
#define FIFO_CTRL_REG 0x2E
#define FIFO_SRC_REG 0x2F
#define BYTES_PER_SAMPLE 6

struct lis3dh {
    struct timer timer;
    uint32_t rest_ticks;
    struct spidev_s *spi;
    uint8_t flags;
    struct sensor_bulk sb;
};


#define FIFO_SRC_REG_WTM(val) ((val)&(1<<7))
#define FIFO_SRC_REG_OVRN(val) ((val)&(1<<6))
#define FIFO_SRC_REG_EMPTY(val) ((val)&(1<<5))
#define FIFO_SRC_REG_FSS(val) ((val)&0x1F)

enum {
    LIS_PENDING = 1<<0,
};

static struct task_wake lis3dh_wake;

// Event handler that wakes lis3dh_task() periodically
static uint_fast8_t
lis3dh_event(struct timer *timer)
{
    struct lis3dh *ax = container_of(timer, struct lis3dh, timer);
    ax->flags |= LIS_PENDING;
    sched_wake_task(&lis3dh_wake);
    return SF_DONE;
}

void
command_config_lis3dh(uint32_t *args)
{
    struct lis3dh *ax = oid_alloc(args[0], command_config_lis3dh
                                   , sizeof(*ax));
    ax->timer.func = lis3dh_event;
    ax->spi = spidev_oid_lookup(args[1]);
}
DECL_COMMAND(command_config_lis3dh, "config_lis3dh oid=%c spi_oid=%c");

// Helper code to reschedule the lis3dh_event() timer
static void
lis3dh_reschedule_timer(struct lis3dh *ax)
{
    irq_disable();
    ax->timer.waketime = timer_read_time() + ax->rest_ticks;
    sched_add_timer(&ax->timer);
    irq_enable();
}

// Query accelerometer data
static void
lis3dh_query(struct lis3dh *ax, uint8_t oid)
{
    uint8_t msg[2] = {0,0};
    uint8_t *d = &ax->sb.data[ax->sb.data_count];

    msg[0] = 0x28 | LIS_AM_READ ;
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    d[0] = msg[1]; // x low bits
    msg[0] = 0x29 | LIS_AM_READ ;
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    d[1] = msg[1]; // x high bits
    msg[0] = 0x2A | LIS_AM_READ ;
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    d[2] = msg[1]; // y low bits
    msg[0] = 0x2B | LIS_AM_READ ;
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    d[3] = msg[1]; // y high bits
    msg[0] = 0x2C | LIS_AM_READ ;
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    d[4] = msg[1]; // z low bits
    msg[0] = 0x2D | LIS_AM_READ ;
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    d[5] = msg[1]; // z high bits

    ax->sb.data_count += BYTES_PER_SAMPLE;
    if (ax->sb.data_count + BYTES_PER_SAMPLE > ARRAY_SIZE(ax->sb.data))
        sensor_bulk_report(&ax->sb, oid);

    uint8_t fifo[2] = {FIFO_SRC_REG| LIS_AM_READ,0};
    spidev_transfer(ax->spi, 1, sizeof(fifo), fifo);
    uint8_t reg_val = fifo[1];
    // Check fifo status
    if (FIFO_SRC_REG_OVRN(reg_val))
        ax->sb.possible_overflows++;

    // check if we need to run the task again (more packets in fifo?)
    if (!FIFO_SRC_REG_EMPTY(reg_val)) {
        // More data in fifo - wake this task again
        sched_wake_task(&lis3dh_wake);
    } else {
        // Sleep until next check time
        ax->flags &= ~LIS_PENDING;
        lis3dh_reschedule_timer(ax);
    }
}

void
command_query_lis3dh(uint32_t *args)
{
    struct lis3dh *ax = oid_lookup(args[0], command_config_lis3dh);

    sched_del_timer(&ax->timer);
    ax->flags = 0;
    if (!args[1])
        // End measurements
        return;

    // Start new measurements query
    ax->rest_ticks = args[1];
    sensor_bulk_reset(&ax->sb);
    lis3dh_reschedule_timer(ax);
}
DECL_COMMAND(command_query_lis3dh, "query_lis3dh oid=%c rest_ticks=%u");

void
command_query_lis3dh_status(uint32_t *args)
{
    struct lis3dh *ax = oid_lookup(args[0], command_config_lis3dh);
    uint8_t msg[2] = { FIFO_SRC_REG | LIS_AM_READ, 0x00 };
    uint32_t time1 = timer_read_time();
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    uint32_t time2 = timer_read_time();
    uint_fast8_t fifo_status =  FIFO_SRC_REG_FSS(msg[1]);
    if(FIFO_SRC_REG_OVRN(msg[1]))
        fifo_status = 32;
    fifo_status&=32;
    sensor_bulk_status(&ax->sb, args[0], time1, time2-time1
                       , fifo_status * BYTES_PER_SAMPLE);
}
DECL_COMMAND(command_query_lis3dh_status, "query_lis3dh_status oid=%c");

void
lis3dh_task(void)
{
    if (!sched_check_wake(&lis3dh_wake))
        return;
    uint8_t oid;
    struct lis3dh *ax;
    foreach_oid(oid, ax, command_config_lis3dh) {
        uint_fast8_t flags = ax->flags;
        if (flags & LIS_PENDING)
            lis3dh_query(ax, oid);
    }
}
DECL_TASK(lis3dh_task);
