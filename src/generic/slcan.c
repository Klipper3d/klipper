// Support for Linux "slcan" CANbus adapter emulation
//
// Copyright (C) 2025 Findlay Feng <i@fengch.me>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"
#include "board/irq.h"
#include "canbus.h"
#include "canserial.h"
#include "command.h"
#include "sched.h"
#include "serial_irq.h"
#include <ctype.h>
#include <string.h>

DECL_CONSTANT("CANBUS_FREQUENCY", CONFIG_CANBUS_FREQUENCY);

#define SLCAN_RX_BUFF_SIZE 128
#define SLCAN_RX_BUFF_MAKS 127

#define SLCAN_TX_QUEUE_SIZE 16
#define SLCAN_TX_QUEUE_MAKS 15

// CAN2.0A
#define SLCAN_MTU (sizeof("t11181122334455667788EA5F\r") + 1)

// CAN2.0B
// #define SLCAN_MTU (sizeof("T1111222281122334455667788EA5F\r") + 1)

union slcan_frame
{
    struct
    {
        char cmd;
        union
        {
            struct
            {
                char id[3];
                char dcl;
                char data[16];
                char timestamp[4];
            } __packed;
            char cmd_data[SLCAN_MTU - 1];
        };
    } __packed;
    char buff[SLCAN_MTU];
};

static struct slcan_data
{
    struct
    {
        uint32_t offset;
        uint32_t count;
        char buff[SLCAN_RX_BUFF_SIZE];
    } rx;

    struct
    {
        char *head;
        uint32_t offset;
        uint32_t count;
        uint32_t working;
        union slcan_frame queue[SLCAN_TX_QUEUE_SIZE];
    } tx;

    uint32_t assigned_id;
} slcan = {0};

static inline uint8_t hex2int(char c)
{
    uint8_t ret = c - '0';

    if (ret < 10)
    {
        return ret;
    }

    return (c - 'A' + 10) & 0xf;
}

static inline char int2hex(uint8_t i)
{
    i &= 0xf;

    return i + (i < 10 ? '0' : 'A' - 10);
}

static int get_canbus_msg_for_slcan_frame(struct canbus_msg *msg,
                                          union slcan_frame const *slcan_f)
{
    msg->id = hex2int(slcan_f->id[0]) << 8 | hex2int(slcan_f->id[1]) << 4 |
              hex2int(slcan_f->id[2]);
    msg->dlc = hex2int(slcan_f->dcl);

    if (msg->dlc > 8)
    {
        return -1;
    }

    char const *data = slcan_f->data;
    for (int i = 0; i < msg->dlc; i++)
    {
        uint8_t tmp = hex2int(*(data++)) << 4;
        msg->data[i] = tmp | hex2int(*(data++));
    }

    return 0;
}

static int get_slcan_frame_for_canbus_msg(union slcan_frame *slcan_f,
                                          struct canbus_msg const *msg)
{
    uint32_t id = msg->id;
    slcan_f->id[0] = int2hex(id >> 8);
    slcan_f->id[1] = int2hex(id >> 4);
    slcan_f->id[2] = int2hex(id);

    uint8_t dlc = msg->dlc;
    if (dlc > 8)
    {
        return -1;
    }

    slcan_f->cmd = 't';
    slcan_f->dcl = int2hex(dlc);

    char *data = slcan_f->data;
    for (int i = 0; i < dlc; i++)
    {
        uint8_t tmp = msg->data[i];
        *(data++) = int2hex(tmp >> 4);
        *(data++) = int2hex(tmp);
    }

    *(data++) = '\r';
    *data = '\0';

    return 0;
}

void serial_rx_byte(uint_fast8_t data)
{
    uint32_t count = slcan.rx.count;
    if (count - slcan.rx.offset >= SLCAN_RX_BUFF_SIZE)
    {
        // todo_error();
        return;
    }

    slcan.rx.buff[(count++) & SLCAN_RX_BUFF_MAKS] = data;
    slcan.rx.count = count;

    if (data == '\r')
    {
        sched_wake_tasks();
    }
}

int serial_get_tx_byte(uint8_t *pdata)
{
    char *head = slcan.tx.head;
    char c = head == NULL ? '\0' : *head;

    while (c == '\0')
    {
        uint32_t offset = slcan.tx.offset;
        if (offset == slcan.tx.count)
        {
            return -1;
        }

        head = slcan.tx.queue[(offset++) & SLCAN_TX_QUEUE_MAKS].buff;
        slcan.tx.offset = offset;
        slcan.tx.head = head;

        c = *head;
    }

    *pdata = c;
    slcan.tx.head = head + 1;
    return 0;
}

static int _slcan_send(int *format(union slcan_frame *slcan_frame,
                                   char const *data),
                       char const *data)
{
    uint32_t working;

    irq_disable();
    if ((slcan.tx.count - (slcan.tx.offset + slcan.tx.working)) >=
        SLCAN_TX_QUEUE_SIZE)
    {
        irq_enable();
        return -1;
    }

    working = slcan.tx.working++;
    irq_enable();

    uint32_t count = slcan.tx.count + working;

    union slcan_frame *f = &slcan.tx.queue[count & SLCAN_TX_QUEUE_MAKS];

    int ret = 0;
    if (format(f, data))
    {
        *f->buff = '\0';
        ret = -2;
    };

    irq_disable();

    if (working == 0)
    {
        slcan.tx.count += slcan.tx.working;
        slcan.tx.working = 0;
        serial_enable_tx_irq();
    }

    irq_enable();

    return ret;
}

static int slcan_frame_fill(union slcan_frame *dest_frame,
                            union slcan_frame const *src_frame)
{
    strncpy((char *)dest_frame, (char const *)src_frame,
            sizeof(union slcan_frame));

    return 0;
}

#define slcan_send(c, f)                                                       \
    _slcan_send((int *(*)(union slcan_frame *, const char *))(c),              \
                (char const *)(f))

static int slcan_send_frame(union slcan_frame const *f)
{
    return slcan_send(slcan_frame_fill, f);
}

static int slcan_send_msg(struct canbus_msg const *msg)
{
    return slcan_send(get_slcan_frame_for_canbus_msg, msg);
}

static const uint32_t slcan_bitrate_const[] = {
    10000, 20000, 50000, 100000, 125000, 250000, 500000, 800000, 1000000};

#define ERROR ((union slcan_frame const *)"\a")
#define OK ((union slcan_frame const *)"\r")

static int frame_dump(union slcan_frame const *f)
{
    switch (f->cmd)
    {
    case 'C':
    case 'O':
        slcan_send_frame(OK);
        break;

    case 'S':
        uint8_t index = f->cmd_data[0] - '0';

        if (index > 8)
        {
            goto error;
        }

        uint32_t s = slcan_bitrate_const[index];

        if (s != CONFIG_CANBUS_FREQUENCY)
        {
            goto error;
        }

        slcan_send_frame(OK);

        break;

    case 't':
        struct canbus_msg msg;
        if (get_canbus_msg_for_slcan_frame(&msg, f) != 0)
        {
            goto error;
        };

        uint32_t id = msg.id;
        if (id == slcan.assigned_id)
        {
            canserial_process_data(&msg);
            slcan_send_frame(OK);
            break;
        }

        if (canhw_send(&msg) < 0)
        {
            return -2;
        }

        slcan_send_frame(OK);
        if (id == CANBUS_ID_ADMIN)
        {
            canserial_process_data(&msg);
        }

        break;

    default:
    error:
        slcan_send_frame(ERROR);
        return -1;
    }

    return 0;
}

static union slcan_frame slcan_tmp = {0};
static char *next = slcan_tmp.buff;
static int sync_next = 0;
void slcan_task(void)
{
    while (slcan.rx.offset != slcan.rx.count)
    {

        uint32_t offset = slcan.rx.offset;

        char c = slcan.rx.buff[offset++ & SLCAN_RX_BUFF_MAKS];

        if (sync_next)
        {
            if (c == '\r')
            {
                sync_next = 0;
            }

            goto next_char;
        }

        if (c == '\r')
        {
            if (next < slcan_tmp.data)
            {
                goto error;
            }

            int dlc = hex2int(slcan_tmp.dcl);

            if (dlc > 8 || next < &slcan_tmp.data[dlc << 1])
            {
                goto next_frame;
            }

            *next = c;
            *(next + 1) = '\0';

            if (frame_dump(&slcan_tmp) == -2)
            {
                sched_wake_tasks();
                return;
            }

            goto next_frame;
        }

        if (!isprint(c))
        {
            goto error;
        }

        if (next - slcan_tmp.buff >= sizeof(slcan_tmp))
        {
            goto error;
        }

        *(next++) = c;

    next_char:
        slcan.rx.offset = offset;
        continue;
    error:
        sync_next = 1;
    next_frame:
        slcan.rx.offset = offset;
        next = slcan_tmp.buff;
    }
}

DECL_TASK(slcan_task);

int canbus_send(struct canbus_msg *msg)
{
    if (slcan_send_msg(msg))
    {
        return -1;
    }

    return msg->dlc;
}

void canbus_set_filter(uint32_t id) { slcan.assigned_id = id; }

void canbus_notify_tx(void) { sched_wake_tasks(); }

void canbus_process_data(struct canbus_msg *msg) { slcan_send_msg(msg); }
