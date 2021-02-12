// Generic handling of serial over CAN support
//
// Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
// Copyright (C) 2020 Pontus Borg <glpontus@gmail.com>
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "canbus.h" // canbus_set_uuid
#include "command.h" // DECL_CONSTANT
#include "sched.h" // sched_wake_task

static uint32_t canbus_assigned_id;
static uint8_t canbus_uuid[CANBUS_UUID_LEN];


/****************************************************************
 * Data transmission over CAN
 ****************************************************************/

static struct task_wake canbus_tx_wake;
static uint8_t transmit_buf[96], transmit_pos, transmit_max;

void
canbus_notify_tx(void)
{
    sched_wake_task(&canbus_tx_wake);
}

void
canbus_tx_task(void)
{
    if (!sched_check_wake(&canbus_tx_wake))
        return;
    uint32_t id = canbus_assigned_id;
    if (!id) {
        transmit_pos = transmit_max = 0;
        return;
    }
    uint32_t tpos = transmit_pos, tmax = transmit_max;
    for (;;) {
        int avail = tmax - tpos, now = avail > 8 ? 8 : avail;
        if (avail <= 0)
            break;
        int ret = canbus_send(id + 1, now, &transmit_buf[tpos]);
        if (ret <= 0)
            break;
        tpos += now;
    }
    transmit_pos = tpos;
}
DECL_TASK(canbus_tx_task);

// Encode and transmit a "response" message
void
console_sendf(const struct command_encoder *ce, va_list args)
{
    // Verify space for message
    uint32_t tpos = transmit_pos, tmax = transmit_max;
    if (tpos >= tmax)
        transmit_pos = transmit_max = tpos = tmax = 0;
    uint32_t max_size = ce->max_size;
    if (tmax + max_size > sizeof(transmit_buf)) {
        if (tmax + max_size - tpos > sizeof(transmit_buf))
            // Not enough space for message
            return;
        // Move buffer
        tmax -= tpos;
        memmove(&transmit_buf[0], &transmit_buf[tpos], tmax);
        transmit_pos = tpos = 0;
        transmit_max = tmax;
    }

    // Generate message
    uint32_t msglen = command_encode_and_frame(&transmit_buf[tmax], ce, args);

    // Start message transmit
    transmit_max = tmax + msglen;
    canbus_notify_tx();
}


/****************************************************************
 * CAN command handling
 ****************************************************************/

static uint8_t receive_buf[192], receive_pos;
DECL_CONSTANT("RECEIVE_WINDOW", ARRAY_SIZE(receive_buf));

static void
can_process_data(uint32_t id, uint32_t len, uint8_t *data)
{
    int rpos = receive_pos;
    if (len > sizeof(receive_buf) - rpos)
        len = sizeof(receive_buf) - rpos;
    memcpy(&receive_buf[rpos], data, len);
    receive_pos = rpos + len;
}

// Helper to retry sending until successful
static void
canbus_send_blocking(uint32_t id, uint32_t len, uint8_t *data)
{
    for (;;) {
        int ret = canbus_send(id, len, data);
        if (ret >= 0)
            return;
    }
}

static void
can_process_ping(uint32_t id, uint32_t len, uint8_t *data)
{
    canbus_send_blocking(canbus_assigned_id + 1, 0, NULL);
}

static void
can_process_reset(uint32_t id, uint32_t len, uint8_t *data)
{
    uint32_t reset_id = data[0] | (data[1] << 8);
    if (reset_id == canbus_assigned_id)
        canbus_reboot();
}

static void
can_process_uuid(uint32_t id, uint32_t len, uint8_t *data)
{
    if (canbus_assigned_id)
        return;
    canbus_send_blocking(CANBUS_ID_UUID_RESP, sizeof(canbus_uuid), canbus_uuid);
}

static void
can_process_set_id(uint32_t id, uint32_t len, uint8_t *data)
{
    // compare my UUID with packet to check if this packet mine
    if (memcmp(&data[2], canbus_uuid, sizeof(canbus_uuid)) == 0) {
        canbus_assigned_id = data[0] | (data[1] << 8);
        canbus_set_filter(canbus_assigned_id);
    }
}

static void
can_process(uint32_t id, uint32_t len, uint8_t *data)
{
    if (id == canbus_assigned_id) {
        if (len)
            can_process_data(id, len, data);
        else
            can_process_ping(id, len, data);
    } else if (id == CANBUS_ID_UUID) {
        if (len)
            can_process_reset(id, len, data);
        else
            can_process_uuid(id, len, data);
    } else if (id==CANBUS_ID_SET) {
        can_process_set_id(id, len, data);
    }
}


/****************************************************************
 * CAN packet reading
 ****************************************************************/

static struct task_wake canbus_rx_wake;

void
canbus_notify_rx(void)
{
    sched_wake_task(&canbus_rx_wake);
}

void
canbus_rx_task(void)
{
    if (!sched_check_wake(&canbus_rx_wake))
        return;

    // Read any pending CAN packets
    for (;;) {
        uint8_t data[8];
        uint32_t id;
        int ret = canbus_read(&id, data);
        if (ret < 0)
            break;
        can_process(id, ret, data);
    }

    // Check for a complete message block and process it
    uint_fast8_t rpos = receive_pos, pop_count;
    int ret = command_find_and_dispatch(receive_buf, rpos, &pop_count);
    if (ret) {
        // Move buffer
        int needcopy = rpos - pop_count;
        if (needcopy) {
            memmove(receive_buf, &receive_buf[pop_count], needcopy);
            canbus_notify_rx();
        }
        rpos = needcopy;
    }
    receive_pos = rpos;
}
DECL_TASK(canbus_rx_task);


/****************************************************************
 * Setup and shutdown
 ****************************************************************/

void
canbus_set_uuid(void *uuid)
{
    memcpy(canbus_uuid, uuid, sizeof(canbus_uuid));
    canbus_notify_rx();

    // Send initial message
    can_process_uuid(0, 0, NULL);
}

void
canbus_shutdown(void)
{
    canbus_notify_tx();
    canbus_notify_rx();
}
DECL_SHUTDOWN(canbus_shutdown);
