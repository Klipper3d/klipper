// Generic interrupt based serial uart helper code
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memmove
#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/io.h" // readb
#include "board/irq.h" // irq_save
#include "board/misc.h" // console_sendf
#include "board/pgm.h" // READP
#include "command.h" // DECL_CONSTANT
#include "sched.h" // sched_wake_tasks
#include "serial_bridge_irq.h" // serial_enable_tx_irq

static uint8_t receive_bridge_buf[SERIAL_BRIDGE_RX_BUFFER_SIZE], receive_bridge_pos;
static uint8_t transmit_bridge_buf[SERIAL_BRIDGE_RX_BUFFER_SIZE], transmit_bridge_pos, transmit_bridge_max;

void serial_bridge_rx_byte(uint_fast8_t data) {
    // if (data == '\r')
    //     return;
    // if (data == '\n') {
    //     sched_wake_tasks();
    // }a
    if (receive_bridge_pos >= SERIAL_BRIDGE_RX_BUFFER_SIZE)
        // Serial overflow - ignore
        return;
    sched_wake_tasks();
    receive_bridge_buf[receive_bridge_pos++] = data;
}

int serial_bridge_get_tx_byte(uint8_t *pdata) {
    if (transmit_bridge_pos >= transmit_bridge_max)
        return -1;
    *pdata = transmit_bridge_buf[transmit_bridge_pos++];
    return 0;
}

void
serial_bridge_send(uint8_t* data, uint_fast8_t size)
{
    // Verify space for message
    uint_fast8_t tpos = readb(&transmit_bridge_pos), tmax = readb(&transmit_bridge_max);
    if (tpos >= tmax) {
        tpos = tmax = 0;
        writeb(&transmit_bridge_max, 0);
        writeb(&transmit_bridge_pos, 0);
    }

    if (tmax + size > sizeof(transmit_bridge_buf)) {
        if (tmax + size - tpos > sizeof(transmit_bridge_buf))
            // Not enough space for message
            return;
        // Disable TX irq and move buffer
        writeb(&transmit_bridge_max, 0);
        tpos = readb(&transmit_bridge_pos);
        tmax -= tpos;
        memmove(&transmit_bridge_buf[0], &transmit_bridge_buf[tpos], tmax);
        writeb(&transmit_bridge_pos, 0);
        writeb(&transmit_bridge_max, tmax);
        serial_bridge_enable_tx_irq();
    }

    // Generate message
    uint8_t *buf = &transmit_bridge_buf[tmax];
    memcpy(buf, data, size);

    // Start message transmit
    writeb(&transmit_bridge_max, tmax + size);
    serial_bridge_enable_tx_irq();
}

// Remove from the receive buffer the given number of bytes
uint8_t
serial_bridge_get_data(uint8_t* data)
{
    for (;;) {
        uint_fast8_t rpos = readb(&receive_bridge_pos);
        if (rpos > SERIAL_BRIDGE_RX_BUFFER_SIZE)
            rpos = 0;
        if (rpos) {
            // Generate message
            uint8_t *buf = receive_bridge_buf;
            memcpy(data, buf, rpos);
            irqstatus_t flag = irq_save();
            if (rpos != readb(&receive_bridge_pos)) {
                // Raced with irq handler - retry
                irq_restore(flag);
                continue;
            }
            receive_bridge_pos = 0;
            irq_restore(flag);
        }
        return rpos;
    }
}
