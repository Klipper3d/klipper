// Generic handling of serial over CAN support
//
// Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
// Copyright (C) 2020 Pontus Borg <glpontus@gmail.com>
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "board/armcm_reset.h" // try_request_canboot
#include "board/io.h" // readb
#include "board/irq.h" // irq_save
#include "board/misc.h" // console_sendf
#include "canbus.h" // canbus_set_uuid
#include "command.h" // DECL_CONSTANT
#include "sched.h" // sched_wake_task

// Global storage
static struct canbus_data {
    uint32_t assigned_id;
    uint8_t uuid[CANBUS_UUID_LEN];

    // Tx data
    struct task_wake tx_wake;
    uint8_t transmit_pos, transmit_max;

    // Rx data
    struct task_wake rx_wake;
    uint8_t receive_pos;

    // Transfer buffers
    uint8_t transmit_buf[96];
    uint8_t receive_buf[192];
} CanData;


/****************************************************************
 * Data transmission over CAN
 ****************************************************************/

void
canbus_notify_tx(void)
{
    sched_wake_task(&CanData.tx_wake);
}

void
canbus_tx_task(void)
{
    if (!sched_check_wake(&CanData.tx_wake))
        return;
    uint32_t id = CanData.assigned_id;
    if (!id) {
        CanData.transmit_pos = CanData.transmit_max = 0;
        return;
    }
    uint32_t tpos = CanData.transmit_pos, tmax = CanData.transmit_max;
    for (;;) {
        int avail = tmax - tpos, now = avail > 8 ? 8 : avail;
        if (avail <= 0)
            break;
        int ret = canbus_send(id + 1, now, &CanData.transmit_buf[tpos]);
        if (ret <= 0)
            break;
        tpos += now;
    }
    CanData.transmit_pos = tpos;
}
DECL_TASK(canbus_tx_task);

// Encode and transmit a "response" message
void
console_sendf(const struct command_encoder *ce, va_list args)
{
    // Verify space for message
    uint32_t tpos = CanData.transmit_pos, tmax = CanData.transmit_max;
    if (tpos >= tmax)
        CanData.transmit_pos = CanData.transmit_max = tpos = tmax = 0;
    uint32_t max_size = ce->max_size;
    if (tmax + max_size > sizeof(CanData.transmit_buf)) {
        if (tmax + max_size - tpos > sizeof(CanData.transmit_buf))
            // Not enough space for message
            return;
        // Move buffer
        tmax -= tpos;
        memmove(&CanData.transmit_buf[0], &CanData.transmit_buf[tpos], tmax);
        CanData.transmit_pos = tpos = 0;
        CanData.transmit_max = tmax;
    }

    // Generate message
    uint32_t msglen = command_encode_and_frame(&CanData.transmit_buf[tmax]
                                               , ce, args);

    // Start message transmit
    CanData.transmit_max = tmax + msglen;
    canbus_notify_tx();
}


/****************************************************************
 * CAN "admin" command handling
 ****************************************************************/

// Available commands and responses
#define CANBUS_CMD_QUERY_UNASSIGNED 0x00
#define CANBUS_CMD_SET_KLIPPER_NODEID 0x01
#define CANBUS_CMD_REQUEST_BOOTLOADER 0x02
#define CANBUS_RESP_NEED_NODEID 0x20

// Helper to verify a UUID in a command matches this chip's UUID
static int
can_check_uuid(uint32_t id, uint32_t len, uint8_t *data)
{
    return len >= 7 && memcmp(&data[1], CanData.uuid, sizeof(CanData.uuid))==0;
}

// Helpers to encode/decode a CAN identifier to a 1-byte "nodeid"
static int
can_get_nodeid(void)
{
    if (!CanData.assigned_id)
        return 0;
    return (CanData.assigned_id - 0x100) >> 1;
}
static uint32_t
can_decode_nodeid(int nodeid)
{
    return (nodeid << 1) + 0x100;
}

static void
can_process_query_unassigned(uint32_t id, uint32_t len, uint8_t *data)
{
    if (CanData.assigned_id)
        return;
    uint8_t send[8];
    send[0] = CANBUS_RESP_NEED_NODEID;
    memcpy(&send[1], CanData.uuid, sizeof(CanData.uuid));
    send[7] = CANBUS_CMD_SET_KLIPPER_NODEID;
    // Send with retry
    for (;;) {
        int ret = canbus_send(CANBUS_ID_ADMIN_RESP, 8, send);
        if (ret >= 0)
            return;
    }
}

static void
can_id_conflict(void)
{
    CanData.assigned_id = 0;
    canbus_set_filter(CanData.assigned_id);
    shutdown("Another CAN node assigned this ID");
}

static void
can_process_set_klipper_nodeid(uint32_t id, uint32_t len, uint8_t *data)
{
    if (len < 8)
        return;
    uint32_t newid = can_decode_nodeid(data[7]);
    if (can_check_uuid(id, len, data)) {
        if (newid != CanData.assigned_id) {
            CanData.assigned_id = newid;
            canbus_set_filter(CanData.assigned_id);
        }
    } else if (newid == CanData.assigned_id) {
        can_id_conflict();
    }
}

static void
can_process_request_bootloader(uint32_t id, uint32_t len, uint8_t *data)
{
    if (!can_check_uuid(id, len, data))
        return;
    try_request_canboot();
}

// Handle an "admin" command
static void
can_process(uint32_t id, uint32_t len, uint8_t *data)
{
    if (!len)
        return;
    switch (data[0]) {
    case CANBUS_CMD_QUERY_UNASSIGNED:
        can_process_query_unassigned(id, len, data);
        break;
    case CANBUS_CMD_SET_KLIPPER_NODEID:
        can_process_set_klipper_nodeid(id, len, data);
        break;
    case CANBUS_CMD_REQUEST_BOOTLOADER:
        can_process_request_bootloader(id, len, data);
        break;
    }
}


/****************************************************************
 * CAN packet reading
 ****************************************************************/

void
canbus_notify_rx(void)
{
    sched_wake_task(&CanData.rx_wake);
}

DECL_CONSTANT("RECEIVE_WINDOW", ARRAY_SIZE(CanData.receive_buf));

// Handle incoming data (called from IRQ handler)
void
canbus_process_data(uint32_t id, uint32_t len, uint8_t *data)
{
    if (!id || id != CanData.assigned_id)
        return;
    int rpos = CanData.receive_pos;
    if (len > sizeof(CanData.receive_buf) - rpos)
        len = sizeof(CanData.receive_buf) - rpos;
    memcpy(&CanData.receive_buf[rpos], data, len);
    CanData.receive_pos = rpos + len;
    canbus_notify_rx();
}

// Remove from the receive buffer the given number of bytes
static void
console_pop_input(int len)
{
    int copied = 0;
    for (;;) {
        int rpos = readb(&CanData.receive_pos);
        int needcopy = rpos - len;
        if (needcopy) {
            memmove(&CanData.receive_buf[copied]
                    , &CanData.receive_buf[copied + len], needcopy - copied);
            copied = needcopy;
            canbus_notify_rx();
        }
        irqstatus_t flag = irq_save();
        if (rpos != readb(&CanData.receive_pos)) {
            // Raced with irq handler - retry
            irq_restore(flag);
            continue;
        }
        CanData.receive_pos = needcopy;
        irq_restore(flag);
        break;
    }
}

// Task to process incoming commands and admin messages
void
canbus_rx_task(void)
{
    if (!sched_check_wake(&CanData.rx_wake))
        return;

    // Read any pending CAN packets
    for (;;) {
        uint8_t data[8];
        uint32_t id;
        int ret = canbus_read(&id, data);
        if (ret < 0)
            break;
        if (id && id == CanData.assigned_id + 1)
            can_id_conflict();
        else if (id == CANBUS_ID_ADMIN)
            can_process(id, ret, data);
    }

    // Check for a complete message block and process it
    uint_fast8_t rpos = readb(&CanData.receive_pos), pop_count;
    int ret = command_find_block(CanData.receive_buf, rpos, &pop_count);
    if (ret > 0)
        command_dispatch(CanData.receive_buf, pop_count);
    if (ret) {
        console_pop_input(pop_count);
        if (ret > 0)
            command_send_ack();
    }
}
DECL_TASK(canbus_rx_task);


/****************************************************************
 * Setup and shutdown
 ****************************************************************/

void
command_get_canbus_id(uint32_t *args)
{
    sendf("canbus_id canbus_uuid=%.*s canbus_nodeid=%u"
          , sizeof(CanData.uuid), CanData.uuid, can_get_nodeid());
}
DECL_COMMAND_FLAGS(command_get_canbus_id, HF_IN_SHUTDOWN, "get_canbus_id");

void
canbus_set_uuid(void *uuid)
{
    memcpy(CanData.uuid, uuid, sizeof(CanData.uuid));
    canbus_notify_rx();
}

void
canbus_shutdown(void)
{
    canbus_notify_tx();
    canbus_notify_rx();
}
DECL_SHUTDOWN(canbus_shutdown);
