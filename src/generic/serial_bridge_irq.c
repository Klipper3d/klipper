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
#include "board/serial_bridge.h" //SERIAL_BRIDGE_CNT

static uint8_t receive_bridge_buf
        [SERIAL_BRIDGE_CNT][SERIAL_BRIDGE_RX_BUFF_SIZE],
        receive_bridge_pos[SERIAL_BRIDGE_CNT];
static uint8_t transmit_bridge_buf
        [SERIAL_BRIDGE_CNT][SERIAL_BRIDGE_RX_BUFF_SIZE],
transmit_bridge_pos[SERIAL_BRIDGE_CNT], transmit_bridge_max[SERIAL_BRIDGE_CNT];

void serial_bridge_rx_byte(uint_fast8_t data, uint8_t usart_index) {
    if (receive_bridge_pos[usart_index] >= SERIAL_BRIDGE_RX_BUFF_SIZE)
        // Serial overflow - ignore
        return;
    sched_wake_tasks();
    receive_bridge_buf[usart_index][receive_bridge_pos[usart_index]++] = data;
}

int serial_bridge_get_tx_byte(uint8_t *pdata, uint8_t usart_index) {
    if (transmit_bridge_pos[usart_index] >= transmit_bridge_max[usart_index])
        return -1;
    *pdata =
        transmit_bridge_buf[usart_index][transmit_bridge_pos[usart_index]++];
    return 0;
}

void
serial_bridge_send(uint8_t* data, uint_fast8_t size, uint8_t config)
{
    uint8_t* usart_index_ptr
        = serial_bridge_get_uart_index_from_config(config);

    if(usart_index_ptr == NULL){
        return;
    }

    uint8_t usart_index = *usart_index_ptr;

    // Verify space for message
    uint_fast8_t tpos =
        readb(&transmit_bridge_pos[usart_index]),
        tmax = readb(&transmit_bridge_max[usart_index]);

    if (tpos >= tmax) {
        tpos = tmax = 0;
        writeb(&transmit_bridge_max[usart_index], 0);
        writeb(&transmit_bridge_pos[usart_index], 0);
    }

    if (tmax + size > sizeof(transmit_bridge_buf[usart_index])) {
        if (tmax + size - tpos > sizeof(transmit_bridge_buf[usart_index]))
            // Not enough space for message
            return;
        // Disable TX irq and move usart_index
        writeb(&transmit_bridge_max[usart_index], 0);
        tpos = readb(&transmit_bridge_pos[usart_index]);
        tmax -= tpos;
        memmove(&transmit_bridge_buf[usart_index][0],
            &transmit_bridge_buf[usart_index][tpos], tmax);
        writeb(&transmit_bridge_pos[usart_index], 0);
        writeb(&transmit_bridge_max[usart_index], tmax);
        serial_bridge_enable_tx_irq(usart_index);
    }

    // Generate message
    uint8_t *buf = &transmit_bridge_buf[usart_index][tmax];
    memcpy(buf, data, size);

    // Start message transmit
    writeb(&transmit_bridge_max[usart_index], tmax + size);
    serial_bridge_enable_tx_irq(usart_index);
}

// Remove from the receive buffer the given number of bytes
uint8_t
serial_bridge_get_data(uint8_t* data, uint8_t config)
{
    uint8_t* usart_index_ptr
        = serial_bridge_get_uart_index_from_config(config);

    if(usart_index_ptr == NULL){
        return 0;
    }

    uint8_t usart_index = *usart_index_ptr;

    for (;;) {
        uint_fast8_t rpos = readb(&receive_bridge_pos[usart_index]);
        if (!rpos)
            return 0;

        uint8_t *buf = receive_bridge_buf[usart_index];
        memcpy(data, buf, rpos);
        irqstatus_t flag = irq_save();
        if (rpos != readb(&receive_bridge_pos[usart_index])) {
            // Raced with irq handler - retry
            irq_restore(flag);
            continue;
        }
        receive_bridge_pos[usart_index] = 0;
        irq_restore(flag);

        return rpos;
    }
}