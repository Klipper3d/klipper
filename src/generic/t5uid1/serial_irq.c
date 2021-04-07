// Generic interrupt based serial uart helper code
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memmove
#include "basecmd.h" // oid_alloc
#include "board/io.h" // readb
#include "board/irq.h" // irq_save
#include "board/misc.h" // timer_read_time
#include "board/t5uid1/serial_irq.h" // t5uid1_enable_tx_irq
#include "command.h" // DECL_CONSTANT
#include "sched.h" // sched_wake_task

#define T5UID1_HEADER1 0x5A
#define T5UID1_HEADER2 0xA5
#define T5UID1_HEADER_LEN 3

#define RX_BUFFER_SIZE 192
#define TIMER_MS 500

struct t5uid1 {
    struct timer timer;
    uint32_t baud;
    uint16_t ticks;
    uint16_t timeout;
    uint8_t timeout_command;
    uint8_t timeout_data_len;
    uint8_t timeout_data[];
};

static uint8_t receive_buf[RX_BUFFER_SIZE], receive_pos;
static uint8_t transmit_buf[96], transmit_pos, transmit_max;

static struct task_wake t5uid1_wake;

void
t5uid1_send_command(uint_fast8_t command, uint8_t *data, uint_fast8_t data_len)
{
    if (data_len < 1)
        return;

    // Verify space for message
    uint_fast8_t tpos = readb(&transmit_pos), tmax = readb(&transmit_max);
    if (tpos >= tmax) {
        tpos = tmax = 0;
        writeb(&transmit_max, 0);
        writeb(&transmit_pos, 0);
    }
    uint_fast8_t msglen = T5UID1_HEADER_LEN + 1 + data_len;
    if (tmax + msglen > sizeof(transmit_buf)) {
        if (tmax + msglen - tpos > sizeof(transmit_buf))
            // Not enough space for message
            return;
        // Disable TX irq and move buffer
        writeb(&transmit_max, 0);
        tpos = readb(&transmit_pos);
        tmax -= tpos;
        memmove(&transmit_buf[0], &transmit_buf[tpos], tmax);
        writeb(&transmit_pos, 0);
        writeb(&transmit_max, tmax);
        t5uid1_enable_tx_irq();
    }

    // Generate message
    transmit_buf[tmax] = T5UID1_HEADER1;
    transmit_buf[tmax + 1] = T5UID1_HEADER2;
    transmit_buf[tmax + 2] = data_len + 1;
    transmit_buf[tmax + 3] = command;
    memcpy(&transmit_buf[tmax + 4], data, data_len);

    // Start message transmit
    writeb(&transmit_max, tmax + msglen);
    t5uid1_enable_tx_irq();
}

static uint_fast8_t
t5uid1_timeout_event(struct timer *timer)
{
    struct t5uid1 *t = container_of(timer, struct t5uid1, timer);
    if (++t->ticks >= t->timeout) {
        if (t->timeout) {
            t5uid1_send_command(t->timeout_command,
                                t->timeout_data, t->timeout_data_len);
        }
        t->ticks = UINT16_MAX;
        return SF_DONE;
    }
    t->timer.waketime += timer_from_us(TIMER_MS * 1000L);
    return SF_RESCHEDULE;
}

void
reset_timer(struct t5uid1 *t)
{
    if (!t->timeout) {
        sched_del_timer(&t->timer);
    } else if (t->ticks == UINT16_MAX) {
        sched_del_timer(&t->timer);
        uint32_t now = timer_read_time();
        t->timer.waketime = now + timer_from_us(TIMER_MS * 1000L);
        t->timer.func = t5uid1_timeout_event;
        if (!timer_is_before(t->timer.waketime, now)) {
            t->ticks = 0;
            sched_add_timer(&t->timer);
        }
    } else {
        t->ticks = 0;
    }
}

void
command_config_t5uid1(uint32_t *args)
{
    uint8_t timeout_data_len = args[4];
    struct t5uid1 *t = oid_alloc(
        args[0], command_config_t5uid1, sizeof(*t) + timeout_data_len);
    t->baud = args[1];
    t->ticks = UINT16_MAX;
    t->timeout = args[2] * (1000 / TIMER_MS);
    t->timeout_command = args[3];
    t->timeout_data_len = timeout_data_len;
    uint8_t *timeout_data = (void*)(size_t)args[5];
    memcpy(t->timeout_data, timeout_data, timeout_data_len);
    t5uid1_init(t->baud);
    reset_timer(t);
}
DECL_COMMAND(command_config_t5uid1,
             "config_t5uid1 oid=%c baud=%u timeout=%hu"
             " timeout_command=%c timeout_data=%*s");

void
command_t5uid1_ping(uint32_t *args)
{
    struct t5uid1 *t = oid_lookup(args[0], command_config_t5uid1);
    reset_timer(t);
}
DECL_COMMAND_FLAGS(command_t5uid1_ping, HF_IN_SHUTDOWN,
                   "t5uid1_ping oid=%c");

void
command_t5uid1_write(uint32_t *args)
{
    struct t5uid1 *t = oid_lookup(args[0], command_config_t5uid1);
    uint_fast8_t command = args[1];
    uint_fast8_t data_len = args[2];
    uint8_t *data = (void*)(size_t)args[3];
    t5uid1_send_command(command, data, data_len);
    reset_timer(t);
}
DECL_COMMAND_FLAGS(command_t5uid1_write, HF_IN_SHUTDOWN,
                   "t5uid1_write oid=%c command=%c data=%*s");

// Rx interrupt - store read data
void
t5uid1_rx_byte(uint_fast8_t data)
{
    if (receive_pos > T5UID1_HEADER_LEN)
        sched_wake_task(&t5uid1_wake);
    if (receive_pos >= sizeof(receive_buf))
        // Serial overflow - ignore it
        return;
    receive_buf[receive_pos++] = data;
}

// Tx interrupt - get next byte to transmit
int
t5uid1_get_tx_byte(uint8_t *pdata)
{
    if (transmit_pos >= transmit_max)
        return -1;
    *pdata = transmit_buf[transmit_pos++];
    return 0;
}

// Remove from the receive buffer the given number of bytes
static void
t5uid1_pop_input(uint_fast8_t len)
{
    uint_fast8_t copied = 0;
    for (;;) {
        uint_fast8_t rpos = readb(&receive_pos);
        uint_fast8_t needcopy = rpos - len;
        if (needcopy) {
            memmove(&receive_buf[copied], &receive_buf[copied + len]
                    , needcopy - copied);
            copied = needcopy;
            sched_wake_task(&t5uid1_wake);
        }
        irqstatus_t flag = irq_save();
        if (rpos != readb(&receive_pos)) {
            // Raced with irq handler - retry
            irq_restore(flag);
            continue;
        }
        receive_pos = needcopy;
        irq_restore(flag);
        break;
    }
}

// Find the next command
int_fast8_t
t5uid1_find_command(uint8_t *buf, uint_fast8_t buf_len, uint_fast8_t *pop_count)
{
    if (buf_len < T5UID1_HEADER_LEN)
        goto need_more_data;
    if (buf[0] != T5UID1_HEADER1 || buf[1] != T5UID1_HEADER2)
        goto error;
    uint_fast8_t cmdlen = buf[2];
    if (cmdlen < 2)
        goto error;
    if (buf_len - T5UID1_HEADER_LEN < cmdlen)
        goto need_more_data;
    *pop_count = T5UID1_HEADER_LEN + cmdlen;
    return 1;

need_more_data:
    *pop_count = 0;
    return 0;
error:
    *pop_count = 1;
    return -1;
}

// Process any incoming commands
void
t5uid1_task(void)
{
    if (!sched_check_wake(&t5uid1_wake))
        return;
    uint_fast8_t rpos = readb(&receive_pos), pop_count;
    int_fast8_t ret = t5uid1_find_command(receive_buf, rpos, &pop_count);
    if (ret > 0) {
        uint_fast8_t command = receive_buf[T5UID1_HEADER_LEN];
        uint_fast8_t data_len = pop_count - T5UID1_HEADER_LEN - 1;
        uint8_t *data = &receive_buf[T5UID1_HEADER_LEN + 1];
        sendf("t5uid1_received command=%c data=%*s", command, data_len, data);
    }
    if (ret) {
        t5uid1_pop_input(pop_count);
    }
}
DECL_TASK(t5uid1_task);
