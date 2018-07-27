// USBSerial on sam4e8e
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// stdlib
#include <stdint.h> //beause of uint_fast8_t in serial_irq.h
#include <string.h>

// Klipper
#include "command.h"
#include "sched.h" // DECL_INIT

// ASF
#include "conf_usb.h"       // Taken from dc42 CoreNG repo
#include "udi_cdc.h"
#include "udc.h"

static volatile bool isConnected;
static uint8_t receive_buf[MESSAGE_MAX], receive_pos;

void
usbserial_init(void)
{
    udc_start();
}
DECL_INIT(usbserial_init);

static void
console_check_input(void)
{
    for (;;) {
        if (receive_pos >= sizeof(receive_buf))
            break;
        iram_size_t ret = udi_cdc_read_no_polling((void*)&receive_buf[receive_pos], (sizeof(receive_buf) - receive_pos));
        if(!ret)
            break;
        receive_pos += ret;
    }
}

static void
console_pop_input(uint8_t len)
{
    uint8_t needcopy = receive_pos - len;
    if (needcopy) {
        memmove(receive_buf, &receive_buf[len], needcopy);
        sched_wake_tasks();
    }
    receive_pos = needcopy;
}

void
console_task(void)
{
    console_check_input();
    uint_fast8_t pop_count;
    int8_t ret = command_find_and_dispatch(receive_buf, receive_pos, &pop_count);
    if (ret)
        console_pop_input(pop_count);

}
DECL_TASK(console_task);

void
console_sendf(const struct command_encoder *ce, va_list args)
{
    while(!udi_cdc_is_tx_ready()) {}
    static uint8_t buf[MESSAGE_MAX];
    uint8_t msglen = command_encode_and_frame(buf, ce, args);
    udi_cdc_write_buf((void*)buf, msglen);
}

bool
core_cdc_enable(uint8_t port)
{
    isConnected = true;
    return true;
}

void
core_cdc_disable(uint8_t port)
{
    isConnected = false;
}

void
core_cdc_rx_notify(uint8_t port)
{
   console_task();
}

void
core_cdc_tx_empty_notify(uint8_t port) {}
