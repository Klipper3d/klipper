// Support for Linux "slcan" CANbus adapter emulation
//
// Copyright (C) 2025 Findlay Feng <i@fengch.me>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"
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

#define SLCAN_TX_QUEUE_SIZE 8
#define SLCAN_TX_QUEUE_MAKS 7

// CAN2.0A
#define SLCAN_MTU (sizeof("t11181122334455667788EA5F\r") + 1)

// CAN2.0B
// #define SLCAN_MTU (sizeof("T1111222281122334455667788EA5F\r") + 1)

#define SLCAN_IDLE 0
#define SLCAN_READY 1

#define SLCAN_TRANSMIT 2
#define SLCAN_NAK 3
#define SLCAN_BELL 4

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
    char c = *(slcan.tx.head++);

    if (c == 0)
    {
        uint32_t offset = slcan.tx.offset;

        slcan.tx.head = slcan.tx.queue[(++offset) & SLCAN_TX_QUEUE_MAKS].buff;
        slcan.tx.offset = offset;
        sched_wake_tasks();

        if (offset == slcan.tx.count)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }

    *pdata = c;

    return 0;
}

// Not supported for use within interrupts
static int _slcan_send(union slcan_frame const *f)
{
    if ((slcan.tx.count - slcan.tx.offset) >= SLCAN_TX_QUEUE_SIZE)
        return -1;

    uint32_t count = slcan.tx.count;

    strcpy(slcan.tx.queue[count & SLCAN_TX_QUEUE_MAKS].buff, f->buff);

    slcan.tx.count = count + 1;

    return 0;
}

// Not supported for use within interrupts
#define slcan_send(f)                                                          \
    while (_slcan_send((f)))                                                   \
    {                                                                          \
        serial_enable_tx_irq();                                                \
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
        slcan_send(OK);
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

        slcan_send(OK);

        break;

    case 't':
        struct canbus_msg msg;
        if (get_canbus_msg_for_slcan_frame(&msg, f) != 0)
        {
            goto error;
        };
        slcan_send(OK);
        slcan_send(f);

        serial_enable_tx_irq();

        if (msg.id == slcan.assigned_id)
        {
            canserial_process_data(&msg);
            break;
        }
        else if (msg.id == CANBUS_ID_ADMIN)
        {
            canserial_process_data(&msg);
        }

        canhw_send(&msg);
        break;

    default:
    error:
        slcan_send(ERROR);
        return -1;
    }

    return 0;
}

static union slcan_frame slcan_tmp = {0};
static char *next = slcan_tmp.buff;
void slcan_task(void)
{
    while (slcan.rx.offset != slcan.rx.count)
    {

        char c = slcan.rx.buff[(slcan.rx.offset)++ & SLCAN_RX_BUFF_MAKS];

        if (!isprint(c) && c != '\r')
        {
            // todo_error;
            goto next_frame;
        }

        if (next >= (slcan_tmp.data + sizeof(slcan_tmp) - 1))
        {
            // todo_error;
            goto next_frame;
        }

        *(next++) = c;
        if (c == '\r')
        {
            *next = '\0';

            frame_dump(&slcan_tmp);

            goto next_frame;
        }

        continue;
    next_frame:
        next = slcan_tmp.buff;
    }
}

DECL_TASK(slcan_task);

int canbus_send(struct canbus_msg *msg)
{
    static union slcan_frame f;

    if (get_slcan_frame_for_canbus_msg(&f, msg) != 0)
    {
        return -1;
    };

    f.cmd = 't';

    slcan_send(&f);

    serial_enable_tx_irq();

    return msg->dlc;
}

void canbus_set_filter(uint32_t id) { slcan.assigned_id = id; }

void canbus_notify_tx(void) { sched_wake_tasks(); }

void canbus_process_data(struct canbus_msg *msg)
{
    static union slcan_frame f;

    if (get_slcan_frame_for_canbus_msg(&f, msg) != 0)
    {
        return;
    };

    f.cmd = 't';

    slcan_send(&f);

    serial_enable_tx_irq();
}
